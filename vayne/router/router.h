#ifndef VAYNE_ROUTER_ROUTER_H
#define VAYNE_ROUTER_ROUTER_H

#include <map>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/atomic.hpp>

#define MQ_QUEUE_MAX_LIMIT 1024

struct RouterMessage
{
	RouterMessage();
	void BuildMessage(const int rid, const char *message, const int size);
	inline std::tuple<int, char *> message() {
		return std::tuple<int, char *>((int)buffer->size(), &buffer->front());
	}
	int rid;
	std::shared_ptr<std::vector<char> > buffer;
};

typedef std::shared_ptr<RouterMessage> RouterMessagePtr;

struct RouterHandler { virtual void do_message(RouterMessagePtr pmessage) = 0; };

/*

Router 是多线程安全的,可以由多个生产者同时发布消息,消息最终被串行处理,由单个消费者处理消息
Process() 的执行是异步的
Router 将会分配一条单独的线程给消费者,生产者和消费者一定属于多个不同的线程
Router 的使用类型必须是 std::shared_ptr<Router> 因为内部使用了 std::shared_from_this()

*/

class Router : public std::enable_shared_from_this<Router>
{
public:
	enum error
	{
		IDR_SUCCESS = 10000,
		// 路由被关闭
		IDR_ROUTER_DISABLE,
		// 注册的数据包已存在
		IDR_REGISTE_PACKET_EXIST,
		// 无效的处理着
		IDR_REGISTER_HANDLER_INVALID,
		// 找不到处理者
		IDR_PROCESS_CANNOT_FOUND_HANDLER,
		// 注册的子路由已存在
		IDR_REGISTER_ROUTER_EXIST,
		// 未处理事件达到了mq的最大上限
		IDR_PROCESS_MAX_LIMIT
	};

	typedef std::shared_ptr<Router> RouterPtr;

	// Router 的父节点和子节点组成了一个路由网络,而单个 Router 表示一个路由设备,对应一张路由表 router_map_
	// 当 router_map_ 没有内容时应该关闭路由功能,因为启动路由需要额外最少一条线程的开销
	// disable_router 并不影响整个路由网络的节点添加和查找,只关闭当前路由设备的消息路由功能

	Router(int rid, bool disable_router = false);

	error Process(const int router_id, std::shared_ptr<RouterMessage> pmessage);

	int RegisterHandler(const int router_id, const RouterHandler *handler_ptr);

	RouterPtr Find(const int id);

	int Append(const RouterPtr &router);

	inline void set_messages_limt(const unsigned int l);

	inline void finish() {
		--untreated_event_count_;
	}

private:

	int id_;
	bool disable_router_;
	std::map<int, RouterHandler *> router_map_;
	std::weak_ptr<Router> parent_;
	std::map<int, RouterPtr> childrens_;

	std::shared_ptr<std::thread> io_thread_;
	boost::asio::io_service ioservice_;
	boost::asio::io_service::work io_work_;
	boost::asio::io_service::strand io_strand_;

	
	boost::atomic<unsigned int > untreated_event_count_;
	unsigned int  max_queue_limit_;
};

typedef Router::RouterPtr RouterPtr;

class RouteFinished {
public:
	RouteFinished(RouterPtr &p);
	~RouteFinished();
private:
	RouterPtr ptr_;
};


#endif
