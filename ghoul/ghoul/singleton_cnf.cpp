#include "StdAfx.h"
#include "singleton_cnf.h"


singleton_cnf::singleton_cnf(void)
{
	
	
}


singleton_cnf::~singleton_cnf(void)
{
}
void singleton_cnf::load()
{
	std::ifstream inf("data/my.json",std::ios::binary | std::ios::in);
	if(!inf.is_open())
	{
		return;
	}
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
	if(!reader.parse(utf8.data(),utf8.data() + utf8.size(),root) || !root.isObject())
	{
		return;
	}
	Json::Value item = root["Run"];
	if(item.isBool() && item.asBool())
	{
		run(true);
	}
	else
	{
		run(false);
	}

	
}
void singleton_cnf::run(bool ok)
{
	if(ok)
	{
		HKEY hKey;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_WRITE,&hKey))
		{
			return;
		}
		wchar_t path[MAX_PATH] = {0};
		if(!GetModuleFileName(NULL,path,MAX_PATH))
		{
			RegCloseKey(hKey);
			return;
		}
		if(ERROR_SUCCESS != RegSetValueEx(hKey, L"dark king's ghoul", 0, REG_SZ, (const BYTE *) path, (wcslen(path)+1) * sizeof(wchar_t)))
		{
			RegCloseKey(hKey);
			return;
		}
		RegCloseKey(hKey);
	}
	else
	{
		HKEY hKey;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE ,&hKey))
		{
			return;
		}
		RegDeleteValue(hKey,L"dark king's ghoul");
		RegCloseKey(hKey);
	}

	_run = ok;
}