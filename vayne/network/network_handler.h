#ifndef VAYNE_NETWORK_NETWORK_HANDLER_H
#define VAYNE_NETWORK_NETWORK_HANDLER_H

#include "timer_count.h"

#include <router/router.h>
#include <boost/asio.hpp>
#include <memory>

class NetworkHandler : public RouterHandler
{

public:
	typedef std::shared_ptr<boost::asio::deadline_timer> timer_ptr_t;


	NetworkHandler::NetworkHandler(RouterPtr &pr);

	void NetworkHandler::do_message(RouterMessagePtr pmessage);

	void time_event(timer_ptr_t &timer);
	std::size_t run(const char *addr, const uint16_t port);
	

	
private:
	std::shared_ptr<TimerCount> timer_count_ptr_;
	RouterPtr network_router_ptr_;
};

#endif