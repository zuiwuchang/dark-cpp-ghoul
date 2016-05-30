#include "StdAfx.h"
#include "strategy_window.h"


strategy_window::strategy_window(CWnd* dlg)
{
	_dlg = dlg;
}


strategy_window::~strategy_window(void)
{
}
bool strategy_window::execute(const std::wstring& cmd)
{
	std::wstring tmp = cmd;
	boost::algorithm::to_lower(tmp);
	if(tmp == L"screen max"){
		_dlg->ShowWindow(SW_SHOWMAXIMIZED);
		return true;
	}else if(tmp == L"screen hide"){
		_dlg->ShowWindow(SW_MINIMIZE);
		_dlg->ShowWindow(SW_HIDE);
		return true;
	}else if(tmp == L"screen restore"){
		_dlg->ModifyStyle(0,WS_CAPTION|WS_THICKFRAME,SWP_FRAMECHANGED);
		_dlg->ShowWindow(SW_RESTORE);
		return true;
	}else if(tmp == L"screen full"){
		_dlg->ModifyStyle(WS_CAPTION|WS_THICKFRAME, 0, SWP_FRAMECHANGED);
		_dlg->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		return true;
	}else if(tmp == L"exit"){
		_dlg->PostMessage(WM_CLOSE);
		return true;
	}
	return false;
}