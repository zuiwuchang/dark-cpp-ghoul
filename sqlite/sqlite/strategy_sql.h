#pragma once
#include "strategy_interface.h"
#include "database.h"
class strategy_sql
	:public strategy_interface
{
public:
	strategy_sql(void);
	~strategy_sql(void);

	virtual bool execute(const std::string& cmd,std::string& out);
	virtual void autocomplete(const std::string& cmd,std::vector<js_autocomplete_node_t>& out);

protected:
	std::vector<std::string> _cmds;

	bool autocomplete_str(const std::string& str,std::vector<js_autocomplete_node_t>& out);
	
	boost::xpressive::sregex _reg_select;
	boost::xpressive::sregex _reg_select_from;
	boost::xpressive::sregex _reg_select_from_table;
	boost::xpressive::sregex _reg_select_wh;
	boost::xpressive::sregex _reg_select_wh_col;

	void autocomplete_select(const std::string& cmd,std::vector<js_autocomplete_node_t>& out);
	void autocomplete_select_from(const std::string& cmd,std::vector<js_autocomplete_node_t>& out);
	void autocomplete_select_table(const std::string& cmd,const std::string& name,std::vector<js_autocomplete_node_t>& out);
	void autocomplete_select_wh(const std::string& cmd,std::vector<js_autocomplete_node_t>& out);

	
};

