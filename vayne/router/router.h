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

Router �Ƕ��̰߳�ȫ��,�����ɶ��������ͬʱ������Ϣ,��Ϣ���ձ����д���,�ɵ��������ߴ�����Ϣ
Process() ��ִ�����첽��
Router �������һ���������̸߳�������,�����ߺ�������һ�����ڶ����ͬ���߳�
Router ��ʹ�����ͱ����� std::shared_ptr<Router> ��Ϊ�ڲ�ʹ���� std::shared_from_this()

*/

class Router : public std::enable_shared_from_this<Router>
{
public:
	enum error
	{
		IDR_SUCCESS = 10000,
		// ·�ɱ��ر�
		IDR_ROUTER_DISABLE,
		// ע������ݰ��Ѵ���
		IDR_REGISTE_PACKET_EXIST,
		// ��Ч�Ĵ�����
		IDR_REGISTER_HANDLER_INVALID,
		// �Ҳ���������
		IDR_PROCESS_CANNOT_FOUND_HANDLER,
		// ע�����·���Ѵ���
		IDR_REGISTER_ROUTER_EXIST,
		// δ�����¼��ﵽ��mq���������
		IDR_PROCESS_MAX_LIMIT
	};

	typedef std::shared_ptr<Router> RouterPtr;

	// Router �ĸ��ڵ���ӽڵ������һ��·������,������ Router ��ʾһ��·���豸,��Ӧһ��·�ɱ� router_map_
	// �� router_map_ û������ʱӦ�ùر�·�ɹ���,��Ϊ����·����Ҫ��������һ���̵߳Ŀ���
	// disable_router ����Ӱ������·������Ľڵ���ӺͲ���,ֻ�رյ�ǰ·���豸����Ϣ·�ɹ���

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
