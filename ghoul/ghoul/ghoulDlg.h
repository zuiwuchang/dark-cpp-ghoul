
// ghoulDlg.h : ͷ�ļ�
//

#pragma once


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
protected:
	inline BOOL CanAccessExternal()
	{
		return TRUE;
	}
	BOOL JsExecute(VARIANT& cmd);
	DECLARE_DISPATCH_MAP()  
};
