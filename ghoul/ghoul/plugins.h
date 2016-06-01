#pragma once
#include <dark-cpp/js/duktape/duktape.hpp>
#include "singleton_cnf.h"

struct js_result
{
	int code;
	std::string msg;

	js_result()
	{
		code = 0;
	}
};
typedef boost::shared_ptr<js_result> js_result_t;
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
	bool _has_show;
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

	inline bool has_show()const
	{
		return _has_show;
	}
	inline bool has_autocomplete()const
	{
		return _has_autocomplete;
	}

protected:
	bool init_modules(module_info_t info);
	bool register_interface();
	bool _has_function(duk_context* ctx,duk_idx_t idx,const char* func);
public:
	js_result_t app_start();
	js_result_t app_stop();
	js_result_t start(const std::string& params);
	js_result_t stop(const std::string& params);
	js_result_t status();
	js_result_t show(const std::string& params);
	void autocomplete(const std::string& params,std::vector<std::string>& outs);
	
};

typedef boost::shared_ptr<plugins> plugins_t;