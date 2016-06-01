#include "StdAfx.h"
#include "events.h"


events::events(void)
{
}


events::~events(void)
{
}
event_info_t events::create_event_info(std::size_t code,const std::wstring& msg,const std::wstring& cmd)
{
	event_info_t rs = boost::make_shared<event_info>();
	rs->code = code;
	rs->msg = msg;
	rs->cmd = cmd;
	return rs;
}
event_info_t events::pop()
{
	boost::mutex::scoped_lock lock(_mutex);
	if(_events.empty())
	{
		return event_info_t();
	}
	event_info_t rs = _events.front();
	_events.pop_front();
	return rs;
}