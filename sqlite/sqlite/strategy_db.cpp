#include "StdAfx.h"
#include "strategy_db.h"
#include "singleton_dbs.h"

strategy_db::strategy_db(void)
	:_opts_open(dark::windows::utf::to_utf8(L"打開/切換到數據庫")),
	_opts_dbs(dark::windows::utf::to_utf8(L"顯示已打開數據庫")),
	_opts_use(dark::windows::utf::to_utf8(L"切換使用的數據庫")),
	_opts_close(dark::windows::utf::to_utf8(L"關閉數據庫連接"))
{
	_reg_open = boost::xpressive::sregex::compile("^open\\s+.+",boost::xpressive::icase);

	
	_opts_open("file","f",dark::windows::utf::to_utf8(L"要打開/切換到的數據庫路徑"),"")
		("key","k",dark::windows::utf::to_utf8(L"數據庫密碼(切換數據庫時無效)"),"")
		("help","h",dark::windows::utf::to_utf8(L"顯示使用說明"),"")
		;
	
	
	_reg_dbs = boost::xpressive::sregex::compile("^dbs.*",boost::xpressive::icase);

	_opts_dbs("id","i",dark::windows::utf::to_utf8(L"顯示所有打開數據庫的工作id"),"")
		("now","n",dark::windows::utf::to_utf8(L"顯示當前數據庫的工作id"),"")
		("file","f",dark::windows::utf::to_utf8(L"顯示文件路徑"),"")
		("help","h",dark::windows::utf::to_utf8(L"顯示使用說明"),"")
		;

	_reg_use = boost::xpressive::sregex::compile("^use\\s+.+",boost::xpressive::icase);
	_opts_use("id","i",dark::windows::utf::to_utf8(L"要切換到目標數據庫工作id"),"")
		("help","h",dark::windows::utf::to_utf8(L"顯示使用說明"),"")
		;

	_reg_close = boost::xpressive::sregex::compile("^close\\s+.+",boost::xpressive::icase);
	_opts_close("id","i",dark::windows::utf::to_utf8(L"要關閉目標數據庫工作id,i=0關閉當前數據庫"),"")
		("all","a",dark::windows::utf::to_utf8(L"關閉全部數據庫"),"")
		("help","h",dark::windows::utf::to_utf8(L"顯示使用說明"),"")
		;
}


strategy_db::~strategy_db(void)
{
}

bool strategy_db::execute(const std::string& cmd,std::string& out)
{
	if(boost::xpressive::regex_match(cmd,_reg_open))
	{
		std::string params = cmd.substr(strlen("open "));
		boost::trim(params);
		dark::strings::options_result_t opts_rs;
		_opts_open.analyze(params,opts_rs);
		std::string path;
		if(opts_rs.has_param("help"))
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_open.write(ostream);
			out = ostream.str();
		}
		else if(opts_rs.has_param("file",path))
		{
			std::string key;
			opts_rs.has_param("key",key);
			std::string msg;
			singleton_dbs::get_mutable_instance().open(path,key,msg);
			out += "<br/>" + msg;
		}
		else
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_open.write(ostream);
			out = ostream.str();
		}
		return true;
	}
	else if(boost::xpressive::regex_match(cmd,_reg_dbs))
	{
		std::string params = cmd.substr(strlen("dbs"));
		boost::trim(params);
		dark::strings::options_result_t opts_rs;
		_opts_dbs.analyze(params,opts_rs);
		
		if(opts_rs.has_param("help"))
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_dbs.write(ostream);
			out = ostream.str();
		}
		else
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			bool path = opts_rs.has_param("file");
			if(opts_rs.has_param("now"))
			{
				database_ptr_t node = singleton_dbs::get_mutable_instance().get_db();
				if(node)
				{
					foreach_dbs(node,path,ostream);
				}
			}
			else
			{
				singleton_dbs::get_mutable_instance().foreach_dbs(
					boost::bind(&strategy_db::foreach_dbs,this,_1,path,boost::ref(ostream))
					);
			}
			out = ostream.str();
		}
		return true;
	}
	else if(boost::xpressive::regex_match(cmd,_reg_use))
	{
		std::string params = cmd.substr(strlen("use"));
		boost::trim(params);
		dark::strings::options_result_t opts_rs;
		_opts_use.analyze(params,opts_rs);
		std::size_t id = 0;
		if(opts_rs.has_param("help"))
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_use.write(ostream);
			out = ostream.str();
		}
		else if(opts_rs.has_param<std::size_t>("id",id))
		{
			std::string msg;
			singleton_dbs::get_mutable_instance().use(id,msg);
			out += "<br/>" + msg;
		}
		else
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_use.write(ostream);
			out = ostream.str();
		}
		return true;
	}
	else if(boost::xpressive::regex_match(cmd,_reg_close))
	{
		std::string params = cmd.substr(strlen("close"));
		boost::trim(params);
		dark::strings::options_result_t opts_rs;
		_opts_close.analyze(params,opts_rs);
		std::size_t id = 0;
		if(opts_rs.has_param("help"))
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_close.write(ostream);
			out = ostream.str();
		}
		else if(opts_rs.has_param<std::size_t>("id",id))
		{
			std::string msg;
			singleton_dbs::get_mutable_instance().close(id,msg);
			out += "<br/>" + msg;
		}
		else if(opts_rs.has_param("all"))
		{
			std::string msg;
			singleton_dbs::get_mutable_instance().close_all(msg);
			out += "<br/>" + msg;
		}
		else
		{
			std::stringstream ostream;
			ostream<<"<br/>";
			_opts_close.write(ostream);
			out = ostream.str();
		}
		return true;
	}
	return false;
}
bool strategy_db::foreach_dbs(database_ptr_t node,bool path,std::stringstream& ostream)
{
	ostream<<node->_id;
	if(path)
	{
		ostream<<"	"<<node->_path<<"\n";
	}
	else
	{
		ostream<<"\n";
	}
	return false;
}
void strategy_db::autocomplete(const std::string& cmd,std::vector<std::string>& out)
{
	std::string tmp = cmd;
	boost::algorithm::to_lower(tmp);
	if(boost::starts_with("dbs ",tmp) ||
		boost::starts_with("dbs	",tmp))
	{
		out.push_back("dbs ");
	}
	else if(boost::starts_with("open ",tmp) ||
		boost::starts_with("open	",tmp))
	{
		out.push_back("open ");
	}
	else if(boost::starts_with("use ",tmp) ||
		boost::starts_with("use	",tmp))
	{
		out.push_back("use ");
	}
	else if(boost::starts_with("close ",tmp) ||
		boost::starts_with("close	",tmp))
	{
		out.push_back("close ");
	}
	
}