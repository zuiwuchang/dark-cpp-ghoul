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
	_cmds.push_back("SELECT * ");
	_reg_select = boost::xpressive::sregex::compile("^select\\s+[a-zA-Z\\*,_]+$",boost::xpressive::icase);
	_reg_select_from = boost::xpressive::sregex::compile("^(select\\s+[a-zA-Z\\*,_]+\\s+)(f|(fr)|(fro)|(from)){0,1}\\s*$",boost::xpressive::icase);
	_reg_select_from_table = boost::xpressive::sregex::compile("^(select\\s+[a-zA-Z\\*,_]+\\s+from\\s+)\\w+$",boost::xpressive::icase);
	_reg_select_wh = boost::xpressive::sregex::compile("^(select\\s+[a-zA-Z\\*,_]+\\s+from\\s+\\w+\\s+)(w|(wh)|(whe)|(wher)(where)){0,1}\\s*$",boost::xpressive::icase);
	

	//table
	_cmds.push_back("CREATE TABLE IF EXISTS _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT)");
	_cmds.push_back("CREATE TABLE IF EXISTS _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT,_ColName TEXT NOT NULL UNIQUE)");
	_cmds.push_back("CREATE TABLE IF EXISTS _TableName (_ColName INTEGER PRIMARY KEY AUTOINCREMENT,_ColName TEXT NOT NULL UNIQUE,FULLTEXT KEY _KeyName(_ColName))");
	_cmds.push_back("DROP TABLE IF EXISTS _TableName");
	
	//alert
	_cmds.push_back("ALTER TABLE _TableName RENAME TO _TableName");
	_cmds.push_back("ALTER TABLE _TableName ADD COLUMN _ColName INTEGER default 0");

	//insert
	_cmds.push_back("INSERT INTO _TableName VALUES ()");

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
	singleton_dbs::get_mutable_instance().execute_sql(cmd,ostream);
	out = ostream.str();
	return true;
}

void strategy_sql::autocomplete(const std::string& cmd,std::vector<js_autocomplete_node_t>& out)
{
	singleton_autocomplete::get_mutable_instance().get_cmds(cmd,
		_cmds,
		boost::bind(&strategy_sql::autocomplete_str,this,_1,boost::ref(out)),
		true);
	
	if(boost::algorithm::istarts_with(cmd,"select"))
	{
		if(boost::xpressive::regex_match(cmd,_reg_select))
		{
			autocomplete_select(cmd,out);
		}
		else if(boost::xpressive::regex_match(cmd,_reg_select_from))
		{
			std::string str = boost::xpressive::regex_replace(cmd,_reg_select_from,"$1");
			autocomplete_select_from(str,out);
		}
		else if(boost::xpressive::regex_match(cmd,_reg_select_from_table))
		{
			std::string str = boost::xpressive::regex_replace(cmd,_reg_select_from_table,"$1");
			std::string name = cmd.substr(str.size());
			autocomplete_select_table(str,name,out);
		}
		else if(boost::xpressive::regex_match(cmd,_reg_select_wh))
		{
			std::string str = boost::xpressive::regex_replace(cmd,_reg_select_wh,"$1");
			autocomplete_select_wh(str,out);
		}
	}

}
bool strategy_sql::autocomplete_str(const std::string& str,std::vector<js_autocomplete_node_t>& out)
{
	js_autocomplete_node_t node = boost::make_shared<autocomplete_node>();
	node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
	node->text = "<div><i>" + str + "</i></div>";
	node->cmd = str;
	out.push_back(node);
	return false;
}
void strategy_sql::autocomplete_select(const std::string& cmd,std::vector<js_autocomplete_node_t>& out)
{
	std::vector<std::string> tables;
	singleton_dbs::get_mutable_instance().get_tables(tables);
	BOOST_FOREACH(const std::string& table,tables)
	{
		std::string str = cmd + " FROM " + table + " ";
		js_autocomplete_node_t node = boost::make_shared<autocomplete_node>();
		node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
		node->text = "<div><i>" + str + "</i></div>";
		node->cmd = str;
		out.push_back(node);
	}
}
void strategy_sql::autocomplete_select_from(const std::string& cmd,std::vector<js_autocomplete_node_t>& out)
{
	std::vector<std::string> tables;
	singleton_dbs::get_mutable_instance().get_tables(tables);
	BOOST_FOREACH(const std::string& table,tables)
	{
		std::string str = cmd + "FROM " + table + " ";
		js_autocomplete_node_t node = boost::make_shared<autocomplete_node>();
		node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
		node->text = "<div><i>" + str + "</i></div>";
		node->cmd = str;
		out.push_back(node);
	}
}
void strategy_sql::autocomplete_select_table(const std::string& cmd,const std::string& name,std::vector<js_autocomplete_node_t>& out)
{
	std::vector<std::string> tables;
	singleton_dbs::get_mutable_instance().get_tables(tables);
	BOOST_FOREACH(const std::string& table,tables)
	{
		if(!boost::algorithm::istarts_with(table,name))
		{
			continue;
		}

		std::string str = cmd + table + " ";
		js_autocomplete_node_t node = boost::make_shared<autocomplete_node>();
		node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
		node->text = "<div><i>" + str + "</i></div>";
		node->cmd = str;
		out.push_back(node);

		str = cmd + table + " WHERE ";
		node = boost::make_shared<autocomplete_node>();
		node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
		node->text = "<div><i>" + str + "</i></div>";
		node->cmd = str;
		out.push_back(node);
	}
}

void strategy_sql::autocomplete_select_wh(const std::string& cmd,std::vector<js_autocomplete_node_t>& out)
{
	std::string str = cmd + "WHERE ";
	js_autocomplete_node_t node = boost::make_shared<autocomplete_node>();
	node->code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE | DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD;
	node->text = "<div><i>" + str + "</i></div>";
	node->cmd = str;
	out.push_back(node);
}
