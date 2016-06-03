#pragma once
#define DARK_SAFE_CLOSE_SQLITE(p) \
    if(p!=NULL){\
        sqlite3_close(p);\
    p=NULL;}
#define DARK_SAFE_DELETE(p)\
	if(p!=NULL){\
        delete p;\
    p=NULL;}
class database
{
public:
	database(void);
	~database(void);

protected:
    sqlite3*  _con;
public:
	std::size_t _id;
	std::string _path;

	bool open(const std::string& path,const std::string& key,std::string& emsg);
	operator sqlite3*()
	{
		return _con;
	}

	void get_tables(std::vector<std::string>& tables);
	bool execute_sql(std::string sql,std::basic_ostream<char>& ostream);
	static std::string html_encryption(std::string str);
};
typedef boost::shared_ptr<database> database_ptr_t;

