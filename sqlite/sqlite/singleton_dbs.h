#pragma once
#include "strategy_interface.h"
#include "database.h"

class singleton_dbs
	: public boost::serialization::singleton<singleton_dbs>
{
public:
	singleton_dbs(void);
	~singleton_dbs(void);

	void app_start();
	void app_stop();

protected:
	std::size_t _id;
	//當前 使用 數據庫
	database_ptr_t _db;
	//已打開 數據庫
	std::map<std::size_t,database_ptr_t> _ids;
	boost::unordered_map<std::string/*path*/,database_ptr_t> _keys;
	
	//可執行命令 策略
	std::vector<strategy_t> _strategys;
public:
	bool execute(const std::string& cmd,std::string& out);
	void autocomplete(const std::string& cmd,std::vector<std::string>& out);

public:
	bool open(const std::string& path,const std::string& key,std::string& msg);
	void foreach_dbs(boost::function<bool(database_ptr_t)> call);
	inline database_ptr_t get_db()const
	{
		return _db;
	}
	bool use(const std::size_t id,std::string& msg);
	bool close(std::size_t id,std::string& msg);
	bool close_all(std::string& msg);

	bool execute_sql(const std::string& sql,std::basic_ostream<char>& ostream);
	static int singleton_dbs::sql_callback(void*lparam,int columns, char **values,char **name);
	static std::string singleton_dbs::html_encryption(std::string str);
};

