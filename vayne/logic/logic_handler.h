#ifndef VAYNE_LOGIC_LOGIC_HANDLER_H
#define VAYNE_LOGIC_LOGIC_HANDLER_H

#include <router/router.h>

class LogicHandler : RouterHandler
{

public:

	LogicHandler(RouterPtr pr);

	void do_message(RouterMessagePtr pmessage);

private:
	RouterPtr logic_router_ptr_;

};

#endif