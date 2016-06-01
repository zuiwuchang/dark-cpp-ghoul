#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CPluginsDialog �Ի���
#include "plugins.h"
#include "strategy_interface.h"

class CPluginsDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CPluginsDialog)

public:
	CPluginsDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPluginsDialog();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_GHOUL_DIALOG, IDH = IDR_HTML_PLUGINS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
	plugins* _plugins;
public:
	inline void SetPlugins(plugins* p)
	{
		_plugins = p;
	}
	//��������ӛ�
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
