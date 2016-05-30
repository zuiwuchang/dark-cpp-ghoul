#pragma once
typedef void (*init_module_t)(duk_context* );
struct module_info
{
	HMODULE module;
	bool js;
	std::wstring path;
	init_module_t init_module; 
	module_info()
	{
		module = NULL;
		js = false;
		init_module = NULL;
	}
	~module_info()
	{
		if(module)
		{
			FreeLibrary(module);
		}
	}
};
typedef boost::shared_ptr<module_info> module_info_t;
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

protected:
	std::vector<module_info_t> _modules;
	std::vector<module_info_t> _plugins;

public:
	void foreach_modules(boost::function<bool(module_info_t)> call);
	void foreach_plugins(boost::function<bool(module_info_t)> call);
};

