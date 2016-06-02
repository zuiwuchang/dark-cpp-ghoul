// sqlite.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "singleton_dbs.h"


duk_ret_t native_name(duk_context *ctx) {
	duk_push_string(ctx,"sqlite");
    return 1;
}
duk_ret_t native_app_start(duk_context *ctx) {
	singleton_dbs::get_mutable_instance().app_start();
	return 0;
}
duk_ret_t native_app_stop(duk_context *ctx) {
	singleton_dbs::get_mutable_instance().app_stop();
    return 0;
}





duk_ret_t native_status(duk_context *ctx) {
	duk_push_object(ctx);
	duk_push_int(ctx, 0);
	duk_put_prop_string(ctx, -2, "code");
	duk_push_string(ctx, "runing");
	duk_put_prop_string(ctx, -2, "msg");
    return 1;
}

duk_ret_t native_show(duk_context *ctx) {
	std::string cmd = duk_require_string(ctx, 0);

	std::string str;
	if(singleton_dbs::get_mutable_instance().execute(cmd,str))
	{
		duk_push_object(ctx);
		duk_push_int(ctx, 0);
		duk_put_prop_string(ctx, -2, "code");
		duk_push_string(ctx, str.c_str());
		duk_put_prop_string(ctx, -2, "msg");
	}
	else
	{
		duk_push_object(ctx);
		duk_push_int(ctx, -1);
		duk_put_prop_string(ctx, -2, "code");
	}
    return 1;
}

duk_ret_t native_autocomplete(duk_context *ctx) {
	std::string cmd = duk_require_string(ctx, 0);

	duk_push_array(ctx);
	std::vector<std::string> outs;
	singleton_dbs::get_mutable_instance().autocomplete(cmd,outs);
	for(std::size_t i=0;i<outs.size();++i)
	{
		duk_push_string(ctx, outs[i].c_str());
		duk_put_prop_index(ctx, -2, i);
	}
	return 1;
}

DARK_EXTERNAL_DECL
void  register_obj(duk_context* ctx)
{
	//實例化 返回插件
	duk_push_object(ctx);

	//設置插件方法
	duk_push_c_function(ctx, native_name,0);
	duk_put_prop_string(ctx, -2, "name");

	duk_push_c_function(ctx, native_app_start,0);
	duk_put_prop_string(ctx, -2, "app_start");

	duk_push_c_function(ctx, native_app_stop,0);
	duk_put_prop_string(ctx, -2, "app_stop");


	duk_push_c_function(ctx, native_status,0);
	duk_put_prop_string(ctx, -2, "status");

	duk_push_c_function(ctx, native_show,1);
	duk_put_prop_string(ctx, -2, "show");

	duk_push_c_function(ctx, native_autocomplete,1);
	duk_put_prop_string(ctx, -2, "autocomplete");
}