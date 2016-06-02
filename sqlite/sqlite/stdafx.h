// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
#include <boost/smart_ptr.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/program_options.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <string>
#include <sstream>
#include <map>

#include <dark-cpp/windows/strings/utf.hpp>
#include <dark-cpp/strings/options.hpp>
#include <dark-cpp/strings/autocomplete.hpp>
typedef boost::serialization::singleton<dark::strings::autocomplete_t> singleton_autocomplete;

#include <duktape-1.5.0/duktape.h>
#ifdef _DEBUG
#pragma comment(lib,"duktape-1.5.0d.lib")
#else
#pragma comment(lib,"duktape-1.5.0.lib")
#endif

#include "../sqlitekey/sqlite3.h"
#ifdef _DEBUG
#pragma comment(lib,"sqlitekeymdd.lib")
#else
#pragma comment(lib,"sqlitekeymd.lib")
#endif

#define DARK_EXTERNAL_DECL extern "C" _declspec(dllexport) 