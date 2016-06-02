#include "StdAfx.h"
#include "strategy_sql.h"
#include "singleton_dbs.h"

strategy_sql::strategy_sql(void)
{
	//_cmds.push_back("PRAGMA TABLE_INFO({{table}})");
	_cmds.push_back("CREATE \\TABLE");
	_cmds.push_back("CREATE \\INDEX");
}


strategy_sql::~strategy_sql(void)
{
}

bool strategy_sql::execute(const std::string& cmd,std::string& out)
{
	std::stringstream ostream;
	ostream<<"<br/>";
	singleton_dbs::get_mutable_instance().execute_sql(cmd,ostream);
	out = ostream.str();
	return true;
}

void strategy_sql::autocomplete(const std::string& cmd,std::vector<std::string>& out)
{
	std::string str = cmd;
	replace_escape(cmd,str);
	boost::xpressive::sregex compare = boost::xpressive::sregex::compile("^" + str + ".*",boost::xpressive::icase);
	BOOST_FOREACH(const std::string& cmd,_cmds)
	{
		if(boost::xpressive::regex_match(cmd,compare))
		{
			out.push_back(cmd);
		}
	}
}
void strategy_sql::replace_escape(const std::string& in,std::string& out)
{
	/*Ìæ“Q \ */
	boost::xpressive::sregex reg = boost::xpressive::sregex::compile("\\\\");
	out = boost::xpressive::regex_replace(in,reg,"\\\\");

	/*Ìæ“Q . */
	boost::xpressive::sregex reg = boost::xpressive::sregex::compile("\\.");
	out = boost::xpressive::regex_replace(out,reg,"\\.");
}