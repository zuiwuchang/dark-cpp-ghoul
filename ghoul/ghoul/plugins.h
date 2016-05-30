#pragma once
#include <dark-cpp/js/duktape/duktape.hpp>
#include "singleton_cnf.h"


class plugins
{
public:
	plugins();
	~plugins(void);
	bool init(module_info_t node);
protected:
	//插件 運行 環境
	dark::js::duktape::context _ctx;

	//插件名
	std::wstring _name;

	//插件存在的 方法
	bool _has_app_start;
	bool _has_app_stop;

	bool _has_start;
	bool _has_stop;

	//_has_status = true
	bool _has_input;
	bool _has_autocomplete;

public:
	inline const std::wstring& name()const
	{
		return _name;
	}
	inline bool has_app_start()const
	{
		return _has_app_start;
	}
	inline bool has_app_stop()const
	{
		return _has_app_stop;
	}

	inline bool has_start()const
	{
		return _has_start;
	}
	inline bool has_stop()const
	{
		return _has_stop;
	}

	inline bool has_input()const
	{
		return _has_input;
	}
	inline bool has_autocomplete()const
	{
		return _has_autocomplete;
	}

protected:
	bool init_modules(module_info_t info);
};

typedef boost::shared_ptr<plugins> plugins_t;