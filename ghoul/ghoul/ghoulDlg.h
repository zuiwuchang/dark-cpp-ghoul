
// ghoulDlg.h : ͷ�ļ�
//

#pragma once
#include "strategy_interface.h"

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
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
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
	BOOL JsExecute(VARIANT& cmd);
	VARIANT JsGetCmds();
	std::vector<strategy_t> _strategys;
	DECLARE_DISPATCH_MAP()  
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	//��������ӛ�
	void SaveCmds();
public:
	afx_msg void OnPopRun();
	afx_msg void OnPopAbout();
	afx_msg void OnPopExit();
};
