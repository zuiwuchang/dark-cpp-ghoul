#pragma once
class strategy_interface
{
public:
	strategy_interface(void);
	~strategy_interface(void);

	virtual bool execute(const std::wstring& cmd) = 0;
};
typedef boost::shared_ptr<strategy_interface> strategy_t;

