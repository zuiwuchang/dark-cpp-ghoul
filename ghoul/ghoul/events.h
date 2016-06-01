#pragma once
#define DARK_EVENT_SHOW_MSG	1
struct event_info
{
	std::size_t code;
	std::wstring msg;
	std::wstring cmd;
};
typedef boost::shared_ptr<event_info> event_info_t;

class events
{
public:
	events(void);
	~events(void);
	static event_info_t create_event_info(std::size_t code,const std::wstring& msg,const std::wstring& cmd);
protected:
	std::list<event_info_t> _events;
	boost::mutex _mutex;
public:
	inline void push(event_info_t e)
	{
		boost::mutex::scoped_lock lock(_mutex);
		_events.push_back(e);
	}
	event_info_t pop();
};
typedef boost::serialization::singleton<events> singleton_events;