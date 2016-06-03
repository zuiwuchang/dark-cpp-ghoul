#pragma once
class strategy_interface
{
public:
	strategy_interface(void);
	~strategy_interface(void);

	virtual bool execute(const std::string& cmd,std::string& out) = 0;
	virtual void autocomplete(const std::string& cmd,std::vector<js_autocomplete_node_t>& out) = 0;
};

typedef boost::shared_ptr<strategy_interface> strategy_t;