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
	virtual void autocomplete(const std::string& cmd,std::vector<std::string>& out);

protected:
	std::vector<std::string> _cmds;
	void replace_escape(const std::string& in,std::string& out);
};

