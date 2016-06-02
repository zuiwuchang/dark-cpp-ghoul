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