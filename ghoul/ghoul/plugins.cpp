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
	_ctx = duk_create_heap_default();
	if(!_ctx)
	{
		return false;
	}
	singleton_cnf& cnf = singleton_cnf::get_mutable_instance();
	cnf.foreach_modules(boost::bind(&plugins::init_modules,this,_1));
	
	return true;
}
bool plugins::init_modules(module_info_t info)
{
#ifdef _DEBUG
	puts("init_modules");
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