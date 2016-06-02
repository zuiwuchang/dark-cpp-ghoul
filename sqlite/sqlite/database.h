#pragma once
#define DARK_SEALF_CLOSE_SQLITE(p) \
    if(p!=NULL){\
        sqlite3_close(p);\
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
};
typedef boost::shared_ptr<database> database_ptr_t;

