#include "stdafx.h"
#include "router.h"
#include <assert.h>
#include <boost/bind.hpp>

RouterMessage::RouterMessage()
{
	buffer.reset(new std::vector<char>);
}
void RouterMessage::BuildMessage(const int rid, const char *message, const int size) {
	this->rid = rid;
	buffer->resize(size);
	memcpy(std::get<1>(this->message()), message, size);
}

Router::Router(int rid, bool disable_router ) :
	id_(rid),
	io_work_(ioservice_),
	io_strand_(ioservice_),
	disable_router_(disable_router),
	max_queue_limit_(MQ_QUEUE_MAX_LIMIT),
	untreated_event_count_(0){

	if (!disable_router_)
		io_thread_.reset(new std::thread(boost::bind(&boost::asio::io_service::run, &ioservice_)));
}

void Router::set_messages_limt(const unsigned int l)
{
	this->max_queue_limit_ = l;
}

Router::error Router::Process(const int router_id, std::shared_ptr<RouterMessage> pmessage) {

	if (++untreated_event_count_.storage() > this->max_queue_limit_) {
		--untreated_event_count_;
		return IDR_PROCESS_MAX_LIMIT;
	}

	std::map<int, RouterHandler *>::iterator iter = router_map_.find(router_id);
	if (iter == router_map_.end()) {
		return IDR_PROCESS_CANNOT_FOUND_HANDLER;
	}
	io_strand_.dispatch(boost::bind(&RouterHandler::do_message, iter->second, pmessage));
	return IDR_SUCCESS;
}

int Router::RegisterHandler(const int router_id, const RouterHandler *handler_ptr) {

	// 路由功能被关闭
	if (this->disable_router_)
		return IDR_ROUTER_DISABLE;

	assert(handler_ptr);

	std::map<int, RouterHandler *>::iterator iter = router_map_.find(router_id);

	assert(iter == router_map_.end());

	if (iter != router_map_.end()) {
		return error::IDR_REGISTE_PACKET_EXIST;
	}
	if (!handler_ptr) {
		return IDR_REGISTER_HANDLER_INVALID;
	}
	router_map_.insert(std::pair<int, RouterHandler *>(router_id, const_cast<RouterHandler *>(handler_ptr)));
	return IDR_SUCCESS;
}

RouterPtr Router::Find(const int id) {
	std::map<int, RouterPtr>::iterator iter = childrens_.find(id);
	return iter != childrens_.end() ? iter->second : NULL;
}

int Router::Append(const RouterPtr &router) {
	if (childrens_.find(router->id_) != childrens_.end())
		return IDR_REGISTER_ROUTER_EXIST;
	router->parent_ = shared_from_this();
	childrens_.insert(std::pair<int, RouterPtr>(router->id_, router));
	return IDR_SUCCESS;
}


RouteFinished::RouteFinished(RouterPtr &p):ptr_(p)
{

}

RouteFinished::~RouteFinished()
{
	if (ptr_)
		ptr_->finish();
}