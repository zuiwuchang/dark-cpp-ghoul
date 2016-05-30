
// ghoulDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ghoul.h"
#include "ghoulDlg.h"
#include "afxdialogex.h"
#include "strategy_window.h"

#include "singleton_cnf.h"

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
	DISP_FUNCTION(CghoulDlg, "GetCmds",  JsGetCmds, VT_VARIANT,VTS_NONE)
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
	ON_COMMAND(ID_POP_RUN, &CghoulDlg::OnPopRun)
	ON_COMMAND(ID_POP_ABOUT, &CghoulDlg::OnPopAbout)
	ON_COMMAND(ID_POP_EXIT, &CghoulDlg::OnPopExit)
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

	singleton_cnf& cnf = singleton_cnf::get_mutable_instance();
	cnf.load();

	//��ʼ�� �ˆ�
	m_menu.LoadMenu(IDR_MENU1);
	if(cnf.run())
	{
		m_menu.CheckMenuItem(ID_POP_RUN,MF_CHECKED);
	}
#ifdef _DEBUG
	SetMenu(&m_menu);
#endif
	//��ʼ�� �D��֪ͨ
	memset(&_notifyicon,0,sizeof(_notifyicon));
    _notifyicon.cbSize = sizeof(NOTIFYICONDATA);
    _notifyicon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    _notifyicon.uCallbackMessage = DARK_MSG_NOTIFYICON;
    _notifyicon.uID = 1;
    _notifyicon.hWnd = m_hWnd;
    wcscpy_s(_notifyicon.szTip,L"ʳ�ƹ�");
    _notifyicon.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	Shell_NotifyIcon(NIM_ADD,&_notifyicon);

	//�]�� �����㷨
	_strategys.push_back(boost::make_shared<strategy_window>(this));
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
BOOL CghoulDlg::JsExecute(VARIANT& cmd)
{
	CString wstr = cmd;
	std::wstring wcs = wstr.GetBuffer();
	BOOST_FOREACH(strategy_t strategy,_strategys)
	{
		if(strategy->execute(wcs))
		{
			return TRUE;
		}
	}
	return FALSE;
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


LRESULT CghoulDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	return CDHtmlDialog::WindowProc(message, wParam, lParam);
}


void CghoulDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	SaveCmds();
	Shell_NotifyIconW(NIM_DELETE,&_notifyicon);
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
