#include "StdAfx.h"
#include "database.h"


database::database(void)
{
	_con = NULL;
}


database::~database(void)
{
	DARK_SAFE_CLOSE_SQLITE(_con)
}
bool database::open(const std::string& path,const std::string& key,std::string& emsg)
{
	if (sqlite3_open(path.c_str(), &_con))
    {
        emsg    =   sqlite3_errmsg(_con);
        DARK_SAFE_CLOSE_SQLITE(_con)
        return false;
    }
    if(!key.empty()
            && sqlite3_key(_con,key.data(),key.size())
      )
    {
        emsg    =   sqlite3_errmsg(_con);
        DARK_SAFE_CLOSE_SQLITE(_con)
        return false;
    }
    char* errmsg = NULL;
    if(SQLITE_OK	!=	sqlite3_exec(_con, "select name from sqlite_master where type = 'table' limit 1" , 0 , 0 , &errmsg))
    {
        emsg    =   errmsg;
        DARK_SAFE_CLOSE_SQLITE(_con);
		DARK_SAFE_DELETE(errmsg);
        return false;
    }
	return true;
}
void database::get_tables(std::vector<std::string>& tables)
{
	char* errmsg = NULL;
	int rows, columns;
    char **results;
    if(SQLITE_OK	!=	sqlite3_get_table(_con, "select name from sqlite_master where type = 'table'" ,&results,&rows,&columns,&errmsg))
    {
        DARK_SAFE_DELETE(errmsg);
        return ;
    }
	
    for (int i = 1; i < rows + 1; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
			tables.push_back(results[i*columns+j]);
        }
    }
	tables.push_back("sqlite_master");
    //·å∑≈results
    sqlite3_free_table(results);

}
bool database::execute_sql(std::string sql,std::basic_ostream<char>& ostream)
{
	char* errmsg = NULL;
	int rows, columns;
    char **results;
	bool col = boost::ends_with(sql,"\\G");
	if(col)
	{
		sql = sql.substr(0,sql.size()-2);
	}
	if(SQLITE_OK	!=	sqlite3_get_table(_con, sql.c_str() ,&results,&rows,&columns,&errmsg))
    {
		ostream<<errmsg;
        DARK_SAFE_DELETE(errmsg);
        return false;
    }
	if(col)
	{
		if(rows)
		{
			int pos = 0;
			ostream<<"<table><tr><th style=\"padding-left: 5px;padding-right: 5px;min-width:150px\">name</th><th style=\"padding-left: 5px;padding-right: 5px;\">value</th></tr>";
			for (int i = 1; i < rows + 1; ++i)
			{	
				ostream<<"<tr><td></td><td>---	"
					<<i
					<<"	---</td></tr>";
				for (int j = 0; j < columns; ++j)
				{
					pos = i * columns + j;
					ostream<<"<tr><td style=\"padding-left: 5px;padding-right: 5px;\">"<<results[j]<<"</td>";
					ostream<<"<td style=\"padding-left: 5px;padding-right: 5px;\">"<<results[pos]<<"</td></tr>";
				}
			}
			ostream<<"</table><div>***	"
				<<rows
				<<" rows	***</div>"
				;
		}
	}
	else
	{
		if(rows)
		{
			int pos = 0;
			for (int i = 0; i < rows + 1; ++i)
			{
				ostream<<"<table><tr>";
				for (int j = 0; j < columns; ++j)
				{
					pos = i * columns + j;

					if(i==0)
					{
						ostream<<"<th style=\"padding-left: 5px;padding-right: 5px;\">";
					}
					else
					{
						ostream<<"<td style=\"padding-left: 5px;padding-right: 5px;\">";
					}
					ostream<<html_encryption((results[pos] ? results[pos] : "NULL"));
					if(i==0)
					{
						ostream<<"</th>";
					}
					else
					{
						ostream<<"</td>";
					}
			
				}
				ostream<<"</tr>";
			}
		
			ostream<<"</table><div>***	"
				<<rows
				<<" rows	***</div>"
				;
		}
	}
	

    //·å∑≈results
    sqlite3_free_table(results);
	return true;
}
std::string database::html_encryption(std::string str)
{
	static std::string left[]= {"\"",
                                "&",
                                "<",
                                ">",
                                " ",

                                "   ",
                                "\n"
                               };
    static std::string right[]= {"&quot;",
                                 "&amp;",
                                 "&lt;",
                                 "&gt;",
                                 "&nbsp;",

                                 "&#9;",
                                 "<br />"
                                };

    for(std::size_t i=0; i<sizeof(left)/sizeof(std::string); ++i)
    {
        if(str.find(left[i]) != std::string::npos)
        {
            boost::xpressive::sregex reg    =   boost::xpressive::sregex::compile(left[i]);

            str = boost::xpressive::regex_replace(str,reg,right[i]);
        }
    }
    return str;
}