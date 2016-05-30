// module_example.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

duk_ret_t my_native_func(duk_context *ctx) {
    MessageBox(NULL,L"this is a js global function(native)",L"example",MB_OK);
    return 0;
}

DARK_EXTERNAL_DECL
void  init_module(duk_context* ctx)
{
	duk_push_global_object(ctx);
	duk_push_c_function(ctx, my_native_func,0);
	duk_put_prop_string(ctx, -2, "module_show");
	duk_pop(ctx);
}

