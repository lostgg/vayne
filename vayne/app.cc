#include "stdafx.h"
#include "app.h"
#include "network/network_handler.h"
#include "logic/logic_handler.h"

#include <boost/log/trivial.hpp>

ServerApp *ServerApp::this_ptr_ = NULL;

ServerApp *ServerApp::GetInctance() {
	if (this_ptr_ == NULL) {
		this_ptr_ = new ServerApp();
	}
	return this_ptr_;
}

ServerApp::ServerApp()
{
}

std::size_t ServerApp::run() {

	// ���ڵ��·��ID�����ںܴ�������Ϊ0
	// ����·�ɹ���,��Ϊ����ֻ����������һ������·������ĸ��ڵ�,��������Ҫ
	message_router_ptr.reset(new Router(0, true));

	// ����ϵͳ�ĺ��߼�ģ��·����
	std::shared_ptr<Router> network_router_ptr(new Router(NETWORK_ROUTER_ID));
	std::shared_ptr<Router> logic_router_ptr(new Router(LOGIC_ROUTER_ID));

	// ��ӵ�·������
	message_router_ptr->Append(network_router_ptr);
	message_router_ptr->Append(logic_router_ptr);

	// ���캯�������·�ɲ���
	std::shared_ptr<NetworkHandler> pnetwork(new NetworkHandler(network_router_ptr));
	std::shared_ptr<LogicHandler> plogic(new LogicHandler(logic_router_ptr));

	// ��������IO����
	return pnetwork->run("127.0.0.1",19900);
}

int ServerApp::asyncall_logic(const int hid, const char *message, const int size) {
	return asyncall_handler(LOGIC_ROUTER_ID, hid, message, size);
}

int ServerApp::asyncall_network(const int hid, const char *message, const int size) {
	return asyncall_handler(NETWORK_ROUTER_ID, hid, message, size);
}

int ServerApp::asyncall_handler(const int rid, const int hid, const char *message, const int size) {
	RouterMessagePtr mmessage(new RouterMessage());
	mmessage->BuildMessage(hid, (const char *)message, size);
	Router::error res;
	while ((res = message_router_ptr->Find(rid)->Process(hid, mmessage)) == Router::error::IDR_PROCESS_MAX_LIMIT) {
		Sleep(2000);
	}
	
	return res;
}