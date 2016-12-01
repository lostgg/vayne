#ifndef VAYNE_NETWORK_NETWORK_HANDLER_H
#define VAYNE_NETWORK_NETWORK_HANDLER_H

#include <router/router.h>

class NetworkHandler : RouterHandler
{

public:

	NetworkHandler::NetworkHandler(RouterPtr &pr);

	void NetworkHandler::do_message(RouterMessagePtr pmessage);

	std::size_t run();
	
private:

	RouterPtr network_router_ptr_;
};

#endif