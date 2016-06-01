#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CPluginsDialog 对话框
#include "plugins.h"
#include "strategy_interface.h"

class CPluginsDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CPluginsDialog)

public:
	CPluginsDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPluginsDialog();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_GHOUL_DIALOG, IDH = IDR_HTML_PLUGINS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
	plugins* _plugins;
public:
	inline void SetPlugins(plugins* p)
	{
		_plugins = p;
	}
	//保存命令記錄
	void SaveCmds();
protected:
	
	inline BOOL CanAccessExternal()
	{
		return TRUE;
	}
	VARIANT JsGetCmds();
	BOOL JsExecute(VARIANT& cmd);
	VARIANT JsGetAutocompletes(VARIANT& cmd);
	void CallJsShowMsg(const std::wstring& msg,const std::wstring& cmd);
public:
	void CallJsSetFocus();
protected:
	std::vector<strategy_t> _strategys;
	boost::xpressive::wsregex _reg_exit;
	boost::xpressive::wsregex _reg_hide;

	virtual void OnOK();


	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
};
