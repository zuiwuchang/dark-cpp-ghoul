
// ghoulDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ghoul.h"
#include "ghoulDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CghoulDlg �Ի���

BEGIN_DHTML_EVENT_MAP(CghoulDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CghoulDlg, CDHtmlDialog)   
    DISP_FUNCTION(CghoulDlg, "Execute",  JsExecute, VT_BOOL, VTS_VARIANT)  
END_DISPATCH_MAP()  

CghoulDlg::CghoulDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CghoulDlg::IDD, CghoulDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CghoulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CghoulDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_DEBUG_RELOAD, &CghoulDlg::OnDebugReload)
END_MESSAGE_MAP()


// CghoulDlg ��Ϣ�������

BOOL CghoulDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	EnableAutomation();
	SetExternalDispatch(GetIDispatch(TRUE));
	
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
BOOL CghoulDlg::JsExecute(VARIANT& cmd)
{
	return FALSE;
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CghoulDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	this->Navigate(L"file:///F:/project/c++/vs2010/dark-cpp-ghoul/ghoul/ghoul/public/views/main1.html");
}
