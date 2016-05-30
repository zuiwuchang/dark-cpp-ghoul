#pragma once
class singleton_cnf
	: public boost::serialization::singleton<singleton_cnf>
{
public:
	singleton_cnf(void);
	~singleton_cnf(void);
	void load();
protected:
	bool _run;
public:
	void run(bool ok);
	inline bool run()const
	{
		return _run;
	}
};

