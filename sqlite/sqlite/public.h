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
#define DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_NO_STYLE		0x1	//���@ʾĬ�J�L�� (�ո� б�w ...)
#define DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD			0x2	//�@ʾ����text �� �Ԅ����cmd ��ͬ
struct autocomplete_node
{
	//����@ʾ
	std::size_t code;
	//�@ʾ�� ������ʾ
	std::string text;
	//�Ԅ���Ɍ� ���� 
	//����]�O�� DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_CMD ���� �t cmd ʹ�� text�ă���
	std::string cmd;

	autocomplete_node()
	{
		code = DARK_PLUGINS_AUTOCOMPLETE_SHOWCODE_DEFAULT;
	}
};
typedef boost::shared_ptr<autocomplete_node> js_autocomplete_node_t;