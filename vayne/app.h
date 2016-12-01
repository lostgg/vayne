
#ifndef VAYNE_APP_H
#define VAYNE_APP_H

#include <router/router.h>



class ServerApp
{
public:

	enum router_id
	{
		NETWORK_ROUTER_ID = 0x10000,
		LOGIC_ROUTER_ID
	};

	static ServerApp *GetInctance();

	std::size_t run();

	int asyncall_logic(const int hid, const char *message, const int size);

	int asyncall_network(const int hid, const char *message, const int size);

	int asyncall_handler(const int rid, const int hid, const char *message, const int size);

private:
	ServerApp();

private:
	// 模块路由 逻辑模块 or 网络模块
	RouterPtr message_router_ptr;
	static ServerApp *this_ptr_;
};
#endif

