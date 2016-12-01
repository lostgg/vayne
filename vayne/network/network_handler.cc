#include "stdafx.h"
#include "network_handler.h"
#include <protocol/protocol_id.h>

#include <app.h>

NetworkHandler::NetworkHandler(RouterPtr &pr) :network_router_ptr_(pr) {

}

void NetworkHandler::do_message(RouterMessagePtr pmessage) {

}

std::size_t NetworkHandler::run() {
	/*while (1) {
		char *buffer = new char[16];
		assert(buffer);
		memset(buffer, 0, 16);
		strcpy_s(buffer, 16, "lostgg");
		ServerApp::GetInctance()->asyncall_logic(IDR_ECHO_STRING, buffer, 16);
		delete buffer;
		Sleep(1);
	}*/
	
	return 0;
}