#pragma once
#include "strategy_interface.h"
class strategy_window
	:public strategy_interface
{
public:
	strategy_window(CWnd* dlg);
	~strategy_window(void);
protected:
	CWnd* _dlg;

	virtual bool execute(const std::wstring& cmd);
};

