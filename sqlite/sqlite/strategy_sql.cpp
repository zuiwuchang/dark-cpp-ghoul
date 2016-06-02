#include "StdAfx.h"
#include "strategy_sql.h"
#include "singleton_dbs.h"

strategy_sql::strategy_sql(void)
{
	//commit
	_cmds.push_back("BEGIN TRANSACTION");
	_cmds.push_back("COMMIT");
	_cmds.push_back("ROLLBACK");

	//index
	_cmds.push_back("DROP INDEX IF EXISTS _IndexName");
	_cmds.push_back("CREATE INDEX IF NOT EXISTS  _IndexName ON _TanleName (_ColName)");
	_cmds.push_back("SELECT * FROM sqlite_master WHERE type = 'index' ORDER BY name");
	_cmds.push_back("SELECT name FROM sqlite_master WHERE type = 'index' ORDER BY name");
	_cmds.push_back("SELECT sql FROM sqlite_master WHERE type = 'index' ORDER BY name");

	//select
	_cmds.push_back("SELECT * FROM _TableName");
	_cmds.push_back("SELECT * FROM _TableName ORDER BY _ColName");
	_cmds.push_back("SELECT * FROM _TableName WHERE _ColName LIKE '%'");
	_cmds.push_back("SELECT * FROM _TableName WHERE _ColName LIKE '.'");
	_cmds.push_back("SELECT * FROM _TableName WHERE _ColName LIKE '%' ORDER BY _ColName");
	_cmds.push_back("SELECT * FROM _TableName WHERE _ColName BETWEEN _Left AND _Right");
	_cmds.push_back("SELECT * FROM _TableName WHERE _ColName BETWEEN _Left AND _Right ORDER BY _ColName");

	//table
	_cmds.push_back("CREATE TABLE _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT)");
	_cmds.push_back("CREATE TABLE _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT,_ColName TEXT NOT NULL UNIQUE)");
	_cmds.push_back("CREATE TABLE _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT,_ColName TEXT NOT NULL UNIQUE,FULLTEXT KEY _KeyName(_ColName))");
	_cmds.push_back("DROP TABLE IF EXISTS _TableName");
	
	//alert
	_cmds.push_back("ALTER TABLE _TableName RENAME TO _TableName");
	_cmds.push_back("ALTER TABLE _TableName ADD COLUMN _ColName INTEGER default 0");

	//system
	_cmds.push_back("PRAGMA TABLE_INFO( _TableName )");
	_cmds.push_back("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name");
	_cmds.push_back("SELECT sql FROM sqlite_master WHERE type = 'table' ORDER BY name");
	_cmds.push_back("SELECT * FROM sqlite_master WHERE type = 'table' ORDER BY name");
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
	singleton_autocomplete::get_mutable_instance().get_cmds(cmd,
		_cmds,
		boost::bind(&strategy_sql::autocomplete_str,this,_1,boost::ref(out)),
		true);
}
bool strategy_sql::autocomplete_str(const std::string& str,std::vector<std::string>& out)
{
	std::string tmp = str + "\n";
	out.push_back(tmp);
	return false;
}