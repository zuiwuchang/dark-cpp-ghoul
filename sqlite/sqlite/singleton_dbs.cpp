#include "StdAfx.h"
#include "singleton_dbs.h"
#include "strategy_db.h"
#include "strategy_sql.h"

singleton_dbs::singleton_dbs(void)
{
	_id = 0;
}


singleton_dbs::~singleton_dbs(void)
{
}
void singleton_dbs::app_start()
{
	_strategys.push_back(boost::make_shared<strategy_db>());
	_strategys.push_back(boost::make_shared<strategy_sql>());
}
void singleton_dbs::app_stop()
{
}
bool singleton_dbs::execute(const std::string& cmd,std::string& out)
{
	BOOST_FOREACH(strategy_t strategy,_strategys)
	{
		if(strategy->execute(cmd,out))
		{
			return true;
		}
	}
	return false;
}
void singleton_dbs::autocomplete(const std::string& cmd,std::vector<js_autocomplete_node_t>& out)
{
	BOOST_FOREACH(strategy_t strategy,_strategys)
	{
		strategy->autocomplete(cmd,out);
	}
}
bool singleton_dbs::open(const std::string& path,const std::string& key,std::string& msg)
{
	//切換數據庫
	BOOST_AUTO(find,_keys.find(path));
	if(find != _keys.end())
	{
		_db = find->second;
		std::size_t id = _db->_id;
		std::stringstream stream;
		stream<<dark::windows::utf::to_utf8(L"切換數據庫	(")<<_db->_id<<")";
		msg = stream.str();
		return true;
	}

	database_ptr_t db = boost::make_shared<database>();
	if(db->open(path,key,msg))
	{
		_db = db;
		_ids[++_id] = db;
		db->_id = _id;
		db->_path = path;
		_keys[path] = db;
		std::stringstream stream;
		stream<<dark::windows::utf::to_utf8(L"新數據庫連接	(")<<_id<<")";
		msg = stream.str();
		return true;
	}

	return false;
}
void singleton_dbs::foreach_dbs(boost::function<bool(database_ptr_t)> call)
{
	BOOST_FOREACH(auto& node,_ids)
	{
		if(call(node.second))
		{
			return;
		}
	}
}
bool singleton_dbs::use(const std::size_t id,std::string& msg)
{
	BOOST_AUTO(find,_ids.find(id));
	if(find != _ids.end())
	{
		_db = find->second;
		std::size_t id = _db->_id;
		std::stringstream stream;
		stream<<dark::windows::utf::to_utf8(L"切換數據庫	(")<<id<<")";
		msg = stream.str();
		return true;
	}
	std::stringstream stream;
	stream<<dark::windows::utf::to_utf8(L"無效的工作id	(")<<id<<")";
	msg = stream.str();
	return false;
}
bool singleton_dbs::close(std::size_t id,std::string& msg)
{
	std::stringstream stream;
	if(id == 0)
	{
		if(_db)
		{
			id = _db->_id;
		}
		else
		{
			stream<<dark::windows::utf::to_utf8(L"沒有在使用的工作數據庫");
			msg = stream.str();
			return false;
		}
	}

	auto find_id = _ids.find(id);
	if(find_id == _ids.end())
	{
		stream<<dark::windows::utf::to_utf8(L"無效的工作id	(")<<id<<")";
		msg = stream.str();
		return false;
	}
	_keys.erase(_keys.find(find_id->second->_path));
	_ids.erase(find_id);
	if(_db && _db->_id == id)
	{
		_db.reset();
	}
	stream<<dark::windows::utf::to_utf8(L"數據庫已關閉	(")<<id<<")";
	msg = stream.str();
	return true;
}
bool singleton_dbs::close_all(std::string& msg)
{
	if(_db)
	{
		_db.reset();
	}
	_keys.clear();
	_ids.clear();
	_id = 0;
	msg = dark::windows::utf::to_utf8(L"數據庫已全部關閉");
	return true;
}
bool singleton_dbs::execute_sql(const std::string& sql,std::basic_ostream<char>& ostream)
{
	if(!_db)
	{
		ostream<<dark::windows::utf::to_utf8(L"沒有在使用的工作數據庫");
		return false;
	}
	_db->execute_sql(sql,ostream);

	return true;
}

void singleton_dbs::get_tables(std::vector<std::string>& tables)
{
	if(_db)
	{
		_db->get_tables(tables);
	}
}