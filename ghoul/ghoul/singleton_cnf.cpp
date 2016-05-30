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
	try
	{
		boost::property_tree::ptree tree;
		boost::property_tree::read_json("data/my.json",tree);

		_run = tree.get<bool>("run",false);

		auto& modules = tree.get_child("modules");
		BOOST_FOREACH(auto& module,modules)
		{
			if(module.second.get<bool>("disable",false))
			{
				continue;
			}
			std::string utf8 = module.second.get<std::string>("path","");
			boost::algorithm::trim(utf8);
			if(utf8.empty())
			{
				continue;
			}
			module_info_t node = boost::make_shared<module_info>();
			node->js = module.second.get<bool>("js",false);
			if(node->js)
			{
				node->path = dark::windows::utf::to_utf16(utf8);
			}
			else
			{
				node->module = LoadLibrary(dark::windows::utf::to_utf16(utf8).c_str());
				if(!node->module)
				{
					continue;
				}
				node->init_module = (init_module_t)GetProcAddress(node->module,"init_module");
				if(!node->init_module)
				{
					continue;
				}
			}
			_modules.push_back(node);
		}

		auto& plugins = tree.get_child("plugins");
		BOOST_FOREACH(auto& plugin,plugins)
		{
			if(plugin.second.get<bool>("disable",false))
			{
				continue;
			}
			std::string utf8 = plugin.second.get<std::string>("path","");
			boost::algorithm::trim(utf8);
			if(utf8.empty())
			{
				continue;
			}
			module_info_t node = boost::make_shared<module_info>();
			node->js = plugin.second.get<bool>("js",false);
			if(node->js)
			{
				node->path = dark::windows::utf::to_utf16(utf8);
			}
			else
			{
				node->module = LoadLibrary(dark::windows::utf::to_utf16(utf8).c_str());
				if(!node->module)
				{
					continue;
				}
			}
			_plugins.push_back(node);
		}
	}
	catch(...)
	{
	}
	run(_run);
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
void singleton_cnf::foreach_modules(boost::function<bool(module_info_t)> call)
{
	BOOST_FOREACH(module_info_t node,_modules)
	{
		if(call(node))
		{
			return;
		}
	}
}
void singleton_cnf::foreach_plugins(boost::function<bool(module_info_t)> call)
{
	BOOST_FOREACH(module_info_t node,_plugins)
	{
		if(call(node))
		{
			return;
		}
	}
}