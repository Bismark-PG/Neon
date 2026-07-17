/*==============================================================================

	Manage Network & Server Sync [Network_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <string>
#pragma comment(lib, "ws2_32.lib")

enum class Network_State
{
	DISCONNECTED,
	CONNECTING,
	CONNECTED
};

class Network_Manager
{
public:
	static Network_Manager& GetInstance()
	{
		static Network_Manager instance;
		return instance;
	}

	bool Init();
	void Final();
	void Update(); // Update Each Frame From Server

	// System Call To Server
	bool Connect_To_Server(const std::string& ip, unsigned short port);
	void Disconnect();
	bool Send_Data(const std::string& data);

	Network_State Get_State() const { return m_State; }

private:
	Network_Manager() = default;
	~Network_Manager() = default;

	SOCKET m_Socket = INVALID_SOCKET;
	Network_State m_State = Network_State::DISCONNECTED;
};

#endif // NETWORK_MANAGER_H