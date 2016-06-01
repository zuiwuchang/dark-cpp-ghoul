#pragma once
#include "strategy_interface.h"
#include "plugins.h"
class strategy_cmds
	:public strategy_interface
{
public:
	strategy_cmds(CWnd* dlg,boost::unordered_map<std::wstring,plugins_t>* plugins);
	~strategy_cmds(void);
protected:
	CWnd* _dlg;
	boost::unordered_map<std::wstring,plugins_t>* _plugins;
	virtual bool execute(const std::wstring& cmd);

	void get_params(const std::wstring& cmd,std::wstring& name,std::wstring& params);

	boost::xpressive::wsregex _reg_start;
	boost::xpressive::wsregex _reg_stop;
	boost::xpressive::wsregex _reg_status;
};

