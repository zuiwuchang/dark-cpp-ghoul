#pragma once

#define DARK_PLUGINS_SHOW_SHOWCODE_DEFAULT		0
#define DARK_PLUGINS_SHOW_SHOWCODE_NO_TIME		0x1
#define DARK_PLUGINS_SHOW_SHOWCODE_NO_CMD		0x2
#define DARK_PLUGINS_SHOW_SHOWCODE_NO_STYLE		0x4
#define DARK_PLUGINS_SHOW_SHOWCODE_NO_CODE		0x8
struct js_result
{
	int code;
	std::string msg;
	std::size_t showcode;
	js_result()
	{
		code = 0;
		showcode = 0;
	}
};
typedef boost::shared_ptr<js_result> js_result_t;

#define DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_DEFAULT		0
#define DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE		0x1	//不顯示默認風格 (空格 斜體 ...)
#define DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD			0x2	//顯示命令text 和 自動完成cmd 不同
struct autocomplete_node
{
	//如何顯示
	std::size_t code;
	//顯示的 命令提示
	std::string text;
	//自動完成對 命令 
	//如果沒設置 DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD 屬性 則 cmd 使用 text的內容
	std::string cmd;

	autocomplete_node()
	{
		code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_DEFAULT;
	}
};
typedef boost::shared_ptr<autocomplete_node> js_autocomplete_node_t;