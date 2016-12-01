#include "stdafx.h"
#include "logic_handler.h"
#include <protocol/protocol_id.h>

#include <iostream>

class EchoHandler : public RouterHandler
{
public:

	EchoHandler(RouterPtr ptr):router_ptr_(ptr){}

	void do_message(RouterMessagePtr pmessage) {
		RouteFinished finished(router_ptr_);
		printf("%s\r\n", std::get<1>(pmessage->message()));
	}

private:

	RouterPtr router_ptr_;
};



LogicHandler::LogicHandler(RouterPtr pr) :logic_router_ptr_(pr) {
	EchoHandler *pp = new EchoHandler(pr);
	// ´òÓ¡×Ö·û´®
	logic_router_ptr_->RegisterHandler(IDR_ECHO_STRING, pp);

}

void LogicHandler::do_message(RouterMessagePtr pmessage) {
}