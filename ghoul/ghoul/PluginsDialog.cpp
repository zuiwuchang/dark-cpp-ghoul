// PluginsDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ghoul.h"
#include "PluginsDialog.h"
#include "strategy_window.h"

// CPluginsDialog 对话框

IMPLEMENT_DYNCREATE(CPluginsDialog, CDHtmlDialog)

CPluginsDialog::CPluginsDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CPluginsDialog::IDD, CPluginsDialog::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

	_reg_exit = boost::xpressive::wsregex::compile(L"^exit$",boost::xpressive::icase);
	_reg_hide = boost::xpressive::wsregex::compile(L"^screen\\s+hide$",boost::xpressive::icase);
}

CPluginsDialog::~CPluginsDialog()
{
}

void CPluginsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CPluginsDialog::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	

	EnableAutomation();
	SetExternalDispatch(GetIDispatch(TRUE));

	//註冊 命令算法
	_strategys.push_back(boost::make_shared<strategy_window>(this));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CPluginsDialog, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CPluginsDialog)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()


BEGIN_DISPATCH_MAP(CPluginsDialog, CDHtmlDialog)   
	DISP_FUNCTION(CPluginsDialog, "Execute",  JsExecute, VT_BOOL, VTS_VARIANT) 
	DISP_FUNCTION(CPluginsDialog, "GetCmds",  JsGetCmds, VT_VARIANT,VTS_NONE)
	DISP_FUNCTION(CPluginsDialog, "GetAutocompletes",  JsGetAutocompletes, VT_VARIANT,VTS_VARIANT)
END_DISPATCH_MAP()  

// CPluginsDialog 消息处理程序

HRESULT CPluginsDialog::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CPluginsDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


void CPluginsDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDHtmlDialog::OnOK();
}
void CPluginsDialog::SaveCmds()
{
	CString dir = L"data";
	CreateDirectory(dir,NULL);
	dir += L"/";
	dir += _plugins->name().c_str();
	CreateDirectory(dir,NULL);
	
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
	std::ofstream outf(dir + L"/cmds.json",std::ios::binary | std::ios::trunc | std::ios::out);
	if(outf.is_open())
	{
		outf.write(utf8.data(),utf8.size());
	}
}
VARIANT CPluginsDialog::JsGetCmds()
{
	CString str = L"[]";
	CString dir = L"data/";
	dir += _plugins->name().c_str();
	std::ifstream inf(dir + L"/cmds.json",std::ios::binary | std::ios::in);
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
BOOL CPluginsDialog::JsExecute(VARIANT& cmd)
{
	CString wstr = cmd;
	std::wstring wcs = wstr.GetBuffer();
	if(boost::xpressive::regex_match(wcs,_reg_exit) || 
		boost::xpressive::regex_match(wcs,_reg_hide))
	{
		OnCancel();
		return TRUE;
	}
	BOOST_FOREACH(strategy_t strategy,_strategys)
	{
		if(strategy->execute(wcs))
		{
			return TRUE;
		}
	}

	js_result_t rs = _plugins->show(dark::windows::utf::to_utf8(wcs));
	if(rs->code == -1)
	{
		return FALSE;
	}

	std::wostringstream out;
	out<<L"("<<rs->code<<L")	"<<dark::windows::utf::to_utf16(rs->msg);
	CallJsShowMsg(out.str(),wcs);
	return TRUE;
}
void CPluginsDialog::CallJsSetFocus()
{
	CComDispatchDriver spScript;
	if(S_OK != m_spHtmlDoc->get_Script(&spScript))
	{
		return;
	}
	spScript.Invoke0(L"set_focus");
}

void CPluginsDialog::CallJsShowMsg(const std::wstring& msg,const std::wstring& cmd)
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

VARIANT CPluginsDialog::JsGetAutocompletes(VARIANT& cmd)
{
	CComVariant rs = L"[]";

	CString wcs = cmd;
	std::string utf8 = dark::windows::utf::to_utf8(wcs.GetBuffer());
	std::vector<std::string> outs;
	_plugins->autocomplete(utf8,outs);
	
	Json::Value arrays;
	BOOST_FOREACH(const std::string& str,outs)
	{
		arrays.append(str);
	}
	if(!arrays.empty())
	{
		rs = dark::windows::utf::to_utf16(arrays.toStyledString()).c_str();

	}
	return rs;
}