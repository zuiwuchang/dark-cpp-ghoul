
// ghoulDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ghoul.h"
#include "ghoulDlg.h"
#include "afxdialogex.h"
#include "strategy_window.h"
#include "strategy_cmds.h"
#include "events.h"


#ifdef _DEBUG
#include <dark/debug/console.hpp>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CghoulDlg 对话框

BEGIN_DHTML_EVENT_MAP(CghoulDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CghoulDlg, CDHtmlDialog)   
    DISP_FUNCTION(CghoulDlg, "Execute",  JsExecute, VT_BOOL, VTS_VARIANT) 
	DISP_FUNCTION(CghoulDlg, "GetCmds",  JsGetCmds, VT_VARIANT,VTS_NONE)
	DISP_FUNCTION(CghoulDlg, "GetAutocompletes",  JsGetAutocompletes, VT_VARIANT,VTS_NONE)
	DISP_FUNCTION(CghoulDlg, "GetEmsg",  JsGetEmsg, VT_VARIANT,VTS_NONE)
END_DISPATCH_MAP()  

CghoulDlg::CghoulDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CghoulDlg::IDD, CghoulDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	_reg_show = boost::xpressive::wsregex::compile(L"^show\\s+.+",boost::xpressive::icase);
}

void CghoulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CghoulDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_DEBUG_RELOAD, &CghoulDlg::OnDebugReload)
	ON_COMMAND(ID_POP_RUN, &CghoulDlg::OnPopRun)
	ON_COMMAND(ID_POP_ABOUT, &CghoulDlg::OnPopAbout)
	ON_COMMAND(ID_POP_EXIT, &CghoulDlg::OnPopExit)
END_MESSAGE_MAP()


// CghoulDlg 消息处理程序

BOOL CghoulDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	EnableAutomation();
	SetExternalDispatch(GetIDispatch(TRUE));

#ifdef _DEBUG
	dark::console::get_mutable_instance().alloc();
#endif

	singleton_cnf& cnf = singleton_cnf::get_mutable_instance();
	cnf.load();

	//初始化 菜單
	m_menu.LoadMenu(IDR_MENU1);
	if(cnf.run())
	{
		m_menu.CheckMenuItem(ID_POP_RUN,MF_CHECKED);
	}
#ifdef _DEBUG
	SetMenu(&m_menu);
#endif
	//初始化 圖標通知
	memset(&_notifyicon,0,sizeof(_notifyicon));
    _notifyicon.cbSize = sizeof(NOTIFYICONDATA);
    _notifyicon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    _notifyicon.uCallbackMessage = DARK_MSG_NOTIFYICON;
    _notifyicon.uID = 1;
    _notifyicon.hWnd = m_hWnd;
    wcscpy_s(_notifyicon.szTip,L"食屍鬼");
    _notifyicon.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	Shell_NotifyIcon(NIM_ADD,&_notifyicon);

	//初始化插件
	cnf.foreach_plugins(boost::bind(&CghoulDlg::InitPlugins,this,_1));



	//註冊 命令算法
	_strategys.push_back(boost::make_shared<strategy_window>(this));
	_strategys.push_back(boost::make_shared<strategy_cmds>(this,&_plugins));
	

	//調用插件 app_start
	BOOST_FOREACH(auto plugins,_plugins)
	{
		js_result_t rs = plugins.second->app_start();
		CString emsg;
		if(rs->code != 0)
		{
			emsg.Format(L"%s (%d)	%s<br>",
				plugins.first.c_str(),
				rs->code,
				dark::windows::utf::to_utf16(rs->msg).c_str());
			_emsg += emsg;
		}
	}
	
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CghoulDlg::JsExecute(VARIANT& cmd)
{
	CString wstr = cmd;
	std::wstring wcs = wstr.GetBuffer();
	if(boost::xpressive::regex_match(wcs,_reg_show))
	{
		std::wstring name = wcs.substr(wcslen(L"show"));
		boost::algorithm::trim(name);
		BOOST_AUTO(find,_dlgs.find(name));
		if(find != _dlgs.end())
		{
			find->second->ShowWindow(SW_SHOW);
			find->second->CallJsSetFocus();
		}
		else
		{
			BOOST_AUTO(find_plugins,_plugins.find(name));
			if(find_plugins == _plugins.end())
			{
				return TRUE;
			}
			plugins_t node = find_plugins->second;
			if(node->has_show())
			{
				boost::shared_ptr<CPluginsDialog> dlg = boost::make_shared<CPluginsDialog>(this);
				if(dlg && dlg->Create(CPluginsDialog::IDD,this))
				{
					dlg->SetPlugins(node.get());
					_dlgs[node->name()] = dlg;
					dlg->SetWindowText(name.c_str());
				}
			}
		}
		return TRUE;
	}
	BOOST_FOREACH(strategy_t strategy,_strategys)
	{
		if(strategy->execute(wcs))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CghoulDlg::CallJsSetFocus()
{
	CComDispatchDriver spScript;
	if(S_OK != m_spHtmlDoc->get_Script(&spScript))
	{
		return;
	}
	spScript.Invoke0(L"set_focus");
}
VARIANT CghoulDlg::JsGetCmds()
{
	CString str = L"[]";
	std::ifstream inf("data/cmds.json",std::ios::binary | std::ios::in);
	if(inf.is_open())
	{
		std::string utf8;
		while(true)
		{
			char c;
			inf.get(c);
			if(inf.eof())
			{
				break;
			}
			utf8.push_back(c);
		}
		Json::Value root;
		Json::Reader reader;
		if(reader.parse(utf8.data(),utf8.data() + utf8.size(),root))
		{
			if(root.isArray())
			{
				str = dark::windows::utf::to_utf16(root.toStyledString()).c_str();
			}
		}
	}

	CComVariant rs = str;
	return rs;
}
VARIANT CghoulDlg::JsGetAutocompletes()
{
	CComVariant rs = L"[]";
	//為插件 添加 命令 提示
	typedef std::pair<std::wstring,plugins_t> plugins_node_t;
	Json::Value arrays;
	std::string tmp;
	BOOST_FOREACH(const plugins_node_t& plugins, _plugins)
	{
		std::string utf8_name = dark::windows::utf::to_utf8(plugins.first);

		tmp = "status " + utf8_name;
		arrays.append(tmp);
		

		if(plugins.second->has_start())
		{
			tmp = "start " + utf8_name;
			arrays.append(tmp);
		}
		if(plugins.second->has_stop())
		{
			tmp = "stop " + utf8_name;
			arrays.append(tmp);
		}

		if(plugins.second->has_show())
		{
			tmp = "show " + utf8_name;
			arrays.append(tmp);
		}
	}
	if(!arrays.empty())
	{
		rs = dark::windows::utf::to_utf16(arrays.toStyledString()).c_str();

	}
	return rs;
}
VARIANT CghoulDlg::JsGetEmsg()
{
	CComVariant rs = _emsg;
	_emsg = L"";
	return rs;
}
void CghoulDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CghoulDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CghoulDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CghoulDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}
void CghoulDlg::OnOK()
{
	return;
}
HRESULT CghoulDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


void CghoulDlg::OnDebugReload()
{
#ifndef _DEBUG
	Navigate(L"file:///F:/project/c++/vs2010/dark-cpp-ghoul/ghoul/ghoul/public/views/main1.html");
#endif
}


LRESULT CghoulDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(DARK_MSG_NOTIFYICON == message)
	{
		if(WM_LBUTTONDBLCLK == lParam)
		{
			if(IsWindowVisible())
			{
				ShowWindow(SW_MINIMIZE);
				ShowWindow(SW_HIDE);
			}
			else
			{
				ModifyStyle(0,WS_CAPTION|WS_THICKFRAME,SWP_FRAMECHANGED);
				ShowWindow(SW_RESTORE);
				SetForegroundWindow();
				CallJsSetFocus();
			}

		}
		else if(WM_RBUTTONDOWN == lParam)
		{
			CPoint point;
			GetCursorPos(&point);
			CMenu *lp = m_menu.GetSubMenu(0);
			lp->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
		}
	}
	else if(DARK_EVENT_NEW == message)
	{
		events& es = singleton_events::get_mutable_instance();
		event_info_t e = es.pop();
		while(e)
		{
			if(DARK_EVENT_SHOW_MSG == e->code)
			{
				CallJsShowMsg(e->msg,e->cmd);
			}
			e = es.pop();
		}
	}
	return CDHtmlDialog::WindowProc(message, wParam, lParam);
}
void CghoulDlg::CallJsShowMsg(const std::wstring& msg,const std::wstring& cmd)
{
	CComDispatchDriver spScript;
	if(S_OK != m_spHtmlDoc->get_Script(&spScript))
	{
		return;
	}
	CComVariant varMsg = msg.c_str();
	CComVariant varCmd = cmd.c_str();
	spScript.Invoke2(L"show_msg",&varMsg,&varCmd);
}

void CghoulDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	SaveCmds();
	Shell_NotifyIconW(NIM_DELETE,&_notifyicon);

	_strategys.clear();
	BOOST_FOREACH(const auto& dlg,_dlgs)
	{
		dlg.second->SaveCmds();
	}

	BOOST_FOREACH(auto plugins,_plugins)
	{
		plugins.second->app_stop();
	}
	_plugins.clear();

	_dlgs.clear();

	CDHtmlDialog::OnCancel();
}
void CghoulDlg::SaveCmds()
{
	CreateDirectory(L"data",NULL);

	CComDispatchDriver spScript;
	if(S_OK != m_spHtmlDoc->get_Script(&spScript))
	{
		return;
	}
	CComVariant varRet;  
	if(S_OK != spScript.Invoke0(L"get_cmds", &varRet))
	{
		return;
	}
	CString str = varRet;
	std::string utf8 = dark::windows::utf::to_utf8(str.GetBuffer());
	std::ofstream outf("data/cmds.json",std::ios::binary | std::ios::trunc | std::ios::out);
	if(outf.is_open())
	{
		outf.write(utf8.data(),utf8.size());
	}
}

void CghoulDlg::OnPopRun()
{
	singleton_cnf& cnf = singleton_cnf::get_mutable_instance();
	if(cnf.run())
	{
		cnf.run(false);
	}
	else
	{
		cnf.run(true);
	}

	if(cnf.run())
	{
		m_menu.CheckMenuItem(ID_POP_RUN,MF_CHECKED);
	}
	else
	{
		m_menu.CheckMenuItem(ID_POP_RUN,MF_UNCHECKED);
	}
	
}


void CghoulDlg::OnPopAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CghoulDlg::OnPopExit()
{
	PostMessage(WM_CLOSE);
}
bool CghoulDlg::InitPlugins(module_info_t info)
{
	plugins_t node = boost::make_shared<plugins>();
	if(node->init(info))
	{
		_plugins[node->name()] = node;
	}
	return false;
}


