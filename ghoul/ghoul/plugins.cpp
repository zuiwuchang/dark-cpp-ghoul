#include "StdAfx.h"
#include "plugins.h"


plugins::plugins()
{
}


plugins::~plugins(void)
{
}
bool plugins::init(module_info_t node)
{
#ifdef _DEBUG
	puts("plugins::init");
#endif
	duk_context* ctx = duk_create_heap_default();
	if(!ctx)
	{
		return false;
	}
	_ctx = ctx;

	//為插件 初始化 js擴展
	singleton_cnf& cnf = singleton_cnf::get_mutable_instance();
	cnf.foreach_modules(boost::bind(&plugins::init_modules,this,_1));
	
	//加載插件
	if(node->js)
	{
		//js 插件
		dark::js::duktape::scoped_pop pop(ctx,1);
		if(duk_peval_file(ctx,dark::windows::utf::to_utf8(node->path).c_str()) != 0)
		{
#ifdef _DEBUG
			 printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
#endif
			 return false;
		}

		//註冊插件	
		duk_push_global_object(ctx);
		++pop;
		
		duk_get_prop_string(ctx, -1, "register_obj");
		if(!duk_is_function(ctx,-1))
		{
			return false;
		}

		if(duk_pcall(ctx,0))
		{
#ifdef _DEBUG
			printf("call register failed: %s\n", duk_safe_to_string(ctx, -1));
#endif
			return false;
		}
		//註冊 插件 接口
		if(!register_interface())
		{
			return false;
		}
		duk_put_prop_string(ctx,-2,"g_instance");
		
	}
	else
	{
		//dll 插件
		duk_push_global_object(ctx);
		dark::js::duktape::scoped_pop pop(ctx,1);

		node->init_module(ctx);

		//註冊 插件 接口
		if(!register_interface())
		{
			return false;
		}
		duk_put_prop_string(ctx,-2,"g_instance");
	}
	
#ifdef _DEBUG
	duk_dump_context_stdout(ctx);
#endif
	return true;
}
bool plugins::register_interface()
{
	std::string utf8;
	duk_context* ctx = _ctx;
	//驗證插件名
	duk_get_prop_string(ctx, -1, "name");
	if(!duk_is_function(ctx,-1))
	{
		return false;
	}
	if(duk_pcall(ctx,0))
	{
		return false;
	}
	if(!duk_is_string(ctx,-1))
	{
		return false;
	}
	utf8 = duk_get_string(ctx,-1);
	boost::algorithm::trim(utf8);
	if(utf8.empty())
	{
		return false;
	}
	_name = dark::windows::utf::to_utf16(utf8);
	duk_pop(ctx);

	//驗證其他接口
	bool has_status = _has_function(ctx,-1,"status");
	if(!has_status)
	{
		return false;
	}
	
	//獲取 接口
	_has_app_start = _has_function(ctx,-1,"app_start");
	_has_app_stop = _has_function(ctx,-1,"app_stop");
	_has_start = _has_function(ctx,-1,"start");
	_has_stop = _has_function(ctx,-1,"stop");
	_has_show = _has_function(ctx,-1,"show");
	_has_autocomplete = _has_function(ctx,-1,"autocomplete");

	return true;
}
bool plugins::_has_function(duk_context* ctx,duk_idx_t idx,const char* func)
{
	bool rs = false;
	duk_get_prop_string(ctx, idx, func);
	if(duk_is_function(ctx,-1))
	{
		rs = true;
	}
	duk_pop(ctx);
	return rs;
}
bool plugins::init_modules(module_info_t info)
{
#ifdef _DEBUG
	puts("plugins::init_modules");
#endif
	duk_context* ctx = _ctx;
	if(info->js)
	{
#ifdef _DEBUG
		dark::js::duktape::scoped_pop pop(ctx,1);
		if(duk_peval_file(ctx,dark::windows::utf::to_utf8(info->path).c_str()) != 0)
		{
			 printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}
#else
		duk_peval_file_noresult(_ctx,dark::windows::utf::to_utf8(info->path).c_str());
#endif
	}
	else
	{
		info->init_module(ctx);
	}

#ifdef _DEBUG
	duk_dump_context_stdout(ctx);
#endif
	return false;
}
js_result_t plugins::app_start()
{
	js_result_t rs = boost::make_shared<js_result>();
	if(has_app_start())
	{
#ifdef _DEBUG
	puts("plugins::app_start");
#endif
		duk_context* ctx = _ctx;
		dark::js::duktape::scoped_pop pop(ctx,3);
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "g_instance");
		duk_get_prop_string(ctx, -1, "app_start");
		duk_get_prop_string(ctx, -3, "g_instance");
		if(duk_pcall_method(ctx,0))
		{
			rs->code = -1;
			rs->msg = duk_to_string(ctx, -1);
		}
		else
		{
			if(duk_is_object(ctx,-1))
			{
				duk_get_prop_string(ctx, -1, "code");
				++pop;
				rs->code = duk_to_int(ctx,-1);
				duk_get_prop_string(ctx, -2, "msg");
				++pop;
				rs->msg = duk_to_string(ctx,-1);
			}
		}
#ifdef _DEBUG
		pop.reset();
		duk_dump_context_stdout(ctx);
#endif
	}
	return rs;
}
js_result_t plugins::app_stop()
{
	js_result_t rs = boost::make_shared<js_result>();
	if(has_app_stop())
	{
#ifdef _DEBUG
	puts("plugins::app_stop");
#endif
		duk_context* ctx = _ctx;
		dark::js::duktape::scoped_pop pop(ctx,3);
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "g_instance");
		duk_get_prop_string(ctx, -1, "app_stop");
		duk_get_prop_string(ctx, -3, "g_instance");
		if(duk_pcall_method(ctx,0))
		{
			rs->code = -1;
			rs->msg = duk_to_string(ctx, -1);
		}
		else
		{
			if(duk_is_object(ctx,-1))
			{
				duk_get_prop_string(ctx, -1, "code");
				++pop;
				rs->code = duk_to_int(ctx,-1);
				duk_get_prop_string(ctx, -2, "msg");
				++pop;
				rs->msg = duk_to_string(ctx,-1);
			}
		}
#ifdef _DEBUG
		pop.reset();
		duk_dump_context_stdout(ctx);
#endif
	}
	return rs;
}
js_result_t plugins::start(const std::string& params)
{
	js_result_t rs = boost::make_shared<js_result>();
	if(has_start())
	{
#ifdef _DEBUG
	puts("plugins::start");
#endif
		duk_context* ctx = _ctx;
		dark::js::duktape::scoped_pop pop(ctx,3);
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "g_instance");
		duk_get_prop_string(ctx, -1, "start");
		duk_get_prop_string(ctx, -3, "g_instance");
		duk_push_string(ctx,params.c_str());
		if(duk_pcall_method(ctx,1))
		{
			rs->code = -1;
			rs->msg = duk_to_string(ctx, -1);
		}
		else
		{
			if(duk_is_object(ctx,-1))
			{
				duk_get_prop_string(ctx, -1, "code");
				++pop;
				rs->code = duk_to_int(ctx,-1);
				duk_get_prop_string(ctx, -2, "msg");
				++pop;
				rs->msg = duk_to_string(ctx,-1);
			}
		}
#ifdef _DEBUG
		pop.reset();
		duk_dump_context_stdout(ctx);
#endif
	}
	return rs;
}
js_result_t plugins::stop(const std::string& params)
{
	js_result_t rs = boost::make_shared<js_result>();
	if(has_stop())
	{
#ifdef _DEBUG
	puts("plugins::stop");
#endif
		duk_context* ctx = _ctx;
		dark::js::duktape::scoped_pop pop(ctx,3);
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "g_instance");
		duk_get_prop_string(ctx, -1, "stop");
		duk_get_prop_string(ctx, -3, "g_instance");
		duk_push_string(ctx,params.c_str());
		if(duk_pcall_method(ctx,1))
		{
			rs->code = -1;
			rs->msg = duk_to_string(ctx, -1);
		}
		else
		{
			if(duk_is_object(ctx,-1))
			{
				duk_get_prop_string(ctx, -1, "code");
				++pop;
				rs->code = duk_to_int(ctx,-1);
				duk_get_prop_string(ctx, -2, "msg");
				++pop;
				rs->msg = duk_to_string(ctx,-1);
			}
		}
#ifdef _DEBUG
		pop.reset();
		duk_dump_context_stdout(ctx);
#endif
	}
	return rs;
}

js_result_t plugins::status()
{
	js_result_t rs = boost::make_shared<js_result>();
	
#ifdef _DEBUG
	puts("plugins::status");
#endif
	duk_context* ctx = _ctx;
	dark::js::duktape::scoped_pop pop(ctx,3);
	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "g_instance");
	duk_get_prop_string(ctx, -1, "status");
	duk_get_prop_string(ctx, -3, "g_instance");
	if(duk_pcall_method(ctx,0))
	{
		rs->code = -1;
		rs->msg = duk_to_string(ctx, -1);
	}
	else
	{
		if(duk_is_object(ctx,-1))
		{
			duk_get_prop_string(ctx, -1, "code");
			++pop;
			rs->code = duk_to_int(ctx,-1);
			duk_get_prop_string(ctx, -2, "msg");
			++pop;
			rs->msg = duk_to_string(ctx,-1);
		}
	}
#ifdef _DEBUG
	pop.reset();
	duk_dump_context_stdout(ctx);
#endif
	
	return rs;
}
js_result_t plugins::show(const std::string& params)
{
	js_result_t rs = boost::make_shared<js_result>();
	if(has_stop())
	{
#ifdef _DEBUG
	puts("plugins::show");
#endif
		duk_context* ctx = _ctx;
		dark::js::duktape::scoped_pop pop(ctx,3);
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "g_instance");
		duk_get_prop_string(ctx, -1, "show");
		duk_get_prop_string(ctx, -3, "g_instance");
		duk_push_string(ctx,params.c_str());
		if(duk_pcall_method(ctx,1))
		{
			rs->code = -1;
			rs->msg = duk_to_string(ctx, -1);
		}
		else
		{
			if(duk_is_object(ctx,-1))
			{
				duk_get_prop_string(ctx, -1, "code");
				++pop;
				rs->code = duk_to_int(ctx,-1);
				duk_get_prop_string(ctx, -2, "msg");
				++pop;
				rs->msg = duk_to_string(ctx,-1);
			}
		}
#ifdef _DEBUG
		pop.reset();
		duk_dump_context_stdout(ctx);
#endif
	}
	return rs;
}
void plugins::autocomplete(const std::string& params,std::vector<std::string>& outs)
{
	if(!has_autocomplete())
	{
		return;
	}
#ifdef _DEBUG
	puts("plugins::autocomplete");
#endif
	duk_context* ctx = _ctx;
	dark::js::duktape::scoped_pop pop(ctx,3);
	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "g_instance");
	duk_get_prop_string(ctx, -1, "autocomplete");
	duk_get_prop_string(ctx, -3, "g_instance");
	duk_push_string(ctx,params.c_str());
	if(!duk_pcall_method(ctx,1))
	{
		if(duk_is_array(ctx,-1))
		{
			for(duk_size_t i=0;i<duk_get_length(ctx,-1);++i)
			{
				duk_get_prop_index(ctx,-1,i);
				outs.push_back(duk_to_string(ctx,-1));
				duk_pop(ctx);
			}
		}
	}
#ifdef _DEBUG
	pop.reset();
	duk_dump_context_stdout(ctx);
#endif
}