#ifndef VAYNE_PROTOCOL_PROTOCOL_ID_H
#define VAYNE_PROTOCOL_PROTOCOL_ID_H

enum logic_module_router_id
{
	IDR_CONNECT_SUCCESS = 0x10000,
	IDR_CONNECT_OFFLINE,
	IDR_USER_LOGIN  ,
	IDR_USRE_REGISTER,
	IDR_USER_GET_PROFILE,
	IDR_ECHO_STRING,
};

enum network_module_router_id
{
	// �Ͽ��û�����
	IDR_CONTROL_DISCONNECT = 0x20000,
	// ����������
	IDR_LIMIT_CONNECT_SIZE,
	// ������Ϣ
	IDR_SEND_MESSAGE,
	// �ڲ���Ϣ
	IDR_INSIDE_PROTO
};

enum network_model_inside_sub_id
{
	// �Ͽ��û�����
	IDR_INSIDE_CHECK_HEART = 0x20000
};


#endif
