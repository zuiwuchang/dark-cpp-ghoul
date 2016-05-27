
// ghoul.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ghoul.h"
#include "ghoulDlg.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CghoulApp

BEGIN_MESSAGE_MAP(CghoulApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CghoulApp ����

CghoulApp::CghoulApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	wchar_t path[MAX_PATH] = {0};
	GetModuleFileName(NULL,path,MAX_PATH);
	boost::filesystem::wpath wpath(path);
	std::wstring name = wpath.filename().c_str();
	
	HKEY hKey = NULL;
	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"SoftWare\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION",
		0,
		KEY_READ|KEY_WRITE,
		&hKey))
	{
		return;
	}

	
	DWORD kType = REG_DWORD;
	DWORD value = 0;
	DWORD size = sizeof(value);
	if(ERROR_SUCCESS != RegQueryValueEx(hKey,
		name.c_str(),
		NULL,
		&kType,
		(LPBYTE)&value,
		&size)
		)
	{
		//������ ����
		value = 0x2711;	//IE10
		RegSetValueEx(hKey, 
			name.c_str(),
			NULL,
			REG_DWORD,
			(LPBYTE)&value,
			size);
	}
	RegCloseKey(hKey);
}


// Ψһ��һ�� CghoulApp ����

CghoulApp theApp;


// CghoulApp ��ʼ��

BOOL CghoulApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CghoulDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

