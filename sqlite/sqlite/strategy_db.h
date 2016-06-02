#pragma once
#include "strategy_interface.h"
#include "database.h"

class strategy_db
	:public strategy_interface
{
public:
	strategy_db(void);
	~strategy_db(void);

	virtual bool execute(const std::string& cmd,std::string& out);
	virtual void autocomplete(const std::string& cmd,std::vector<std::string>& out);

protected:
	boost::xpressive::sregex _reg_open;
	dark::strings::options_t _opts_open;

	boost::xpressive::sregex _reg_dbs;
	dark::strings::options_t _opts_dbs;

	boost::xpressive::sregex _reg_use;
	dark::strings::options_t _opts_use;

	boost::xpressive::sregex _reg_close;
	dark::strings::options_t _opts_close;

	bool foreach_dbs(database_ptr_t node,bool path,std::stringstream& ostream);

	std::vector<std::string> _cmds;
};

