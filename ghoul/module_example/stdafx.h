// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <duktape-1.5.0/duktape.h>
#ifdef _DEBUG
#pragma comment(lib,"duktape-1.5.0d.lib")
#else
#pragma comment(lib,"duktape-1.5.0.lib")
#endif

#define DARK_EXTERNAL_DECL extern "C" _declspec(dllexport) 



