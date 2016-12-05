#include "stdafx.h"
#include "network_handler.h"

#include <base/package.hpp>
#include <protocol/protocol_id.h>
#include <protocol/net/inside_proto.h>

#include <app.h>

#include <iostream>
#include <memory>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

class CheckHeartHandler: public RouterHandler
{
public:
	CheckHeartHandler(RouterPtr &r):r_(r) {

	}

	void do_message(RouterMessagePtr pmessage)
	{
		RouteFinished f(r_);
		std::string str;
		auto message = pmessage->message();
		str.assign(std::get<1>(message), std::get<0>(message));
		InsideProto pro;
		lfcd::PacketOp::UppackStream(pro, str);

		printf("%08x\r\n", pro.type());
		//std::cout << pro.type() << std::endl;
	}
private:
	RouterPtr r_;
};

NetworkHandler::NetworkHandler(RouterPtr &pr) :network_router_ptr_(pr) {
	timer_count_ptr_.reset(new TimerCount);
	CheckHeartHandler *ppp = new CheckHeartHandler(network_router_ptr_);
	network_router_ptr_->RegisterHandler(IDR_INSIDE_PROTO, ppp);
}

void NetworkHandler::do_message(RouterMessagePtr pmessage) {

}

void NetworkHandler::time_event(timer_ptr_t &timer)
{
	std::cout << "timer run" << std::endl;
	timer_count_ptr_->AddAll();

	if (timer_count_ptr_->tick_count >= 5) {
		timer_count_ptr_->tick_count = 0;
		InsideProto inside; // 投递一个检测客户端心跳的请求
		inside.set_type(IDR_INSIDE_CHECK_HEART);
		std::string str = lfcd::PacketOp::PackStream(inside);
		ServerApp::GetInctance()->asyncall_network(IDR_INSIDE_PROTO, str.c_str(), str.length());
	}

	timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));
	timer->async_wait(boost::bind(&NetworkHandler::time_event, this, timer));
}

std::size_t NetworkHandler::run(const char *addr, const uint16_t port) {
	

	// 初始化时间计数器
	timer_count_ptr_->Init();

	boost::asio::io_service service;
	timer_ptr_t timer_ptr(new  boost::asio::deadline_timer(service, boost::posix_time::seconds(1)));

	timer_ptr->async_wait(boost::bind( &NetworkHandler::time_event,this,timer_ptr));
	//timer.async_wait();

	return service.run();
}