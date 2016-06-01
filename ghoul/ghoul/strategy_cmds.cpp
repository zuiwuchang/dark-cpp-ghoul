#include "StdAfx.h"
#include "strategy_cmds.h"
#include "events.h"


strategy_cmds::strategy_cmds(CWnd* dlg,boost::unordered_map<std::wstring,plugins_t>* plugins)
{
	_dlg = dlg;
	_plugins = plugins;

	_reg_start = boost::xpressive::wsregex::compile(L"^start\\s+.+",boost::xpressive::icase);
	_reg_stop = boost::xpressive::wsregex::compile(L"^stop\\s+.+",boost::xpressive::icase);
	_reg_status = boost::xpressive::wsregex::compile(L"^status\\s+.+",boost::xpressive::icase);
}


strategy_cmds::~strategy_cmds(void)
{
}

bool strategy_cmds::execute(const std::wstring& cmd)
{
	if(boost::xpressive::regex_match(cmd,_reg_start))
	{
		std::wstring name = cmd.substr(wcslen(L"start"));
		boost::trim(name);
		std::wstring plugins,params;
		get_params(name,plugins,params);

		auto find = _plugins->find(plugins);
		if(find != _plugins->end())
		{
			js_result_t rs = find->second->start(dark::windows::utf::to_utf8(params));
			std::wostringstream out;
			out<<L"("<<rs->code<<L")	"<<dark::windows::utf::to_utf16(rs->msg);
			event_info_t e = events::create_event_info(DARK_EVENT_SHOW_MSG, out.str(),cmd);
			singleton_events::get_mutable_instance().push(e);
			_dlg->PostMessage(DARK_EVENT_NEW);
			return true;
		}
		return false;
	}
	else if(boost::xpressive::regex_match(cmd,_reg_stop))
	{
		std::wstring name = cmd.substr(wcslen(L"stop"));
		boost::trim(name);
		std::wstring plugins,params;
		get_params(name,plugins,params);

		auto find = _plugins->find(plugins);
		if(find != _plugins->end())
		{
			js_result_t rs = find->second->stop(dark::windows::utf::to_utf8(params));
			std::wostringstream out;
			out<<L"("<<rs->code<<L")	"<<dark::windows::utf::to_utf16(rs->msg);
			event_info_t e = events::create_event_info(DARK_EVENT_SHOW_MSG, out.str(),cmd);
			singleton_events::get_mutable_instance().push(e);
			_dlg->PostMessage(DARK_EVENT_NEW);
			return true;
		}
		return false;
	}
	else if(boost::xpressive::regex_match(cmd,_reg_status))
	{
		std::wstring name = cmd.substr(wcslen(L"status"));
		boost::trim(name);
		

		auto find = _plugins->find(name);
		if(find != _plugins->end())
		{
			js_result_t rs = find->second->status();
			std::wostringstream out;
			out<<L"("<<rs->code<<L")	"<<dark::windows::utf::to_utf16(rs->msg);
			event_info_t e = events::create_event_info(DARK_EVENT_SHOW_MSG, out.str(),cmd);
			singleton_events::get_mutable_instance().push(e);
			_dlg->PostMessage(DARK_EVENT_NEW);
			return true;
		}
		return false;
	}

	return false;
}
void strategy_cmds::get_params(const std::wstring& cmd,std::wstring& name,std::wstring& params)
{
	BOOST_AUTO(find,cmd.find(L' '));
	BOOST_AUTO(find_tab,cmd.find(L'	'));
	if(find_tab != std::wstring::npos)
	{
		if(find == std::wstring::npos)
		{
			find = find_tab;
		}
		else if(find_tab < find)
		{
			find = find_tab;
		}
	}

	if(find == std::wstring::npos)
	{
		name = cmd;
	}
	else
	{
		name = cmd.substr(0,find);
		params = cmd.substr(find);
		boost::algorithm::trim(name);
		boost::algorithm::trim(params);
	}
}