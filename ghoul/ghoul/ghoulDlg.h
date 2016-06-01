
// ghoulDlg.h : ͷ�ļ�
//

#pragma once
#include "strategy_interface.h"
#include "plugins.h"
#include "singleton_cnf.h"
#include "PluginsDialog.h"

// CghoulDlg �Ի���
class CghoulDlg : public CDHtmlDialog
{
// ����
public:
	CghoulDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GHOUL_DIALOG, IDH = IDR_HTML_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
public:
	afx_msg void OnDebugReload();

	virtual void OnOK();
	NOTIFYICONDATA _notifyicon;
	CMenu m_menu;
protected:
	inline BOOL CanAccessExternal()
	{
		return TRUE;
	}
	boost::xpressive::wsregex _reg_show;
	BOOL JsExecute(VARIANT& cmd);
	VARIANT JsGetCmds();
	VARIANT JsGetAutocompletes();
	std::vector<strategy_t> _strategys;
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	//��������ӛ�
	void SaveCmds();

	void CallJsSetFocus();
	void CallJsShowMsg(const std::wstring& msg,const std::wstring& cmd);
public:
	afx_msg void OnPopRun();
	afx_msg void OnPopAbout();
	afx_msg void OnPopExit();

	boost::unordered_map<std::wstring,plugins_t> _plugins;
	bool InitPlugins(module_info_t info);

	boost::unordered_map<std::wstring,boost::shared_ptr<CPluginsDialog>> _dlgs;



	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()  
	
};
