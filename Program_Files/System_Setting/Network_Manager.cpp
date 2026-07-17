/*==============================================================================

	Manage Network & Server Sync [Network_Manager.cpp]

==============================================================================*/
#include "Project_Header.h"
#include "Network_Manager.h"
#include "debug_ostream.h"
#include <WS2tcpip.h>

bool Network_Manager::Init()
{
	WSADATA wsaData;
	// Winsock Init
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Debug::D_Out << "[Network Manager] WSAStartup Failed!" << std::endl;
		return false;
	}

	m_State = Network_State::DISCONNECTED;
	Debug::D_Out << "[Network Manager] System Initialized." << std::endl;
	return true;
}

void Network_Manager::Final()
{
	Disconnect();
	WSACleanup();
	Debug::D_Out << "[Network Manager] System Finalized." << std::endl;
}

bool Network_Manager::Connect_To_Server(const std::string& ip, unsigned short port)
{
	if (m_State == Network_State::CONNECTED) return true;

	// 1. Make Socket
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		Debug::D_Out << "[Network Manager] Socket Creation Error!" << std::endl;
		return false;
	}

	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

	// 2. Try Connect Server
	// But Now, Try When Touch UI Only, So Blocking Accept
	if (connect(m_Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		Debug::D_Out << "[Network Manager] Connect Failed!" << std::endl;
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		return false;
	}

	// 3. If Connect Success, Change Socket "Non-Blocking" Mode
	// If Do Not Change Mode, Game Will Be Crack In Update
	u_long mode = 1;
	ioctlsocket(m_Socket, FIONBIO, &mode);

	m_State = Network_State::CONNECTED;
	Debug::D_Out << "[Network Manager] Connected to Server: " << ip << ":" << port << std::endl;
	
	return true;
}

void Network_Manager::Disconnect()
{
	if (m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	m_State = Network_State::DISCONNECTED;
	Debug::D_Out << "[Network Manager] Disconnected." << std::endl;
}

bool Network_Manager::Send_Data(const std::string& data)
{
	if (m_State != Network_State::CONNECTED || m_Socket == INVALID_SOCKET) return false;

	// Send Data To Server
	int result = send(m_Socket, data.c_str(), static_cast<int>(data.length()), 0);
	if (result == SOCKET_ERROR)
	{
		Debug::D_Out << "[Network Manager] Send Error!" << std::endl;
		Disconnect();
		return false;
	}
	return true;
}

void Network_Manager::Update()
{
	// If Not Connect, Do Not Check (Update)
	if (m_State != Network_State::CONNECTED || m_Socket == INVALID_SOCKET) return;

	char buffer[1024];
	ZeroMemory(buffer, sizeof(buffer));

	// Chack Data From Server
	// If No Data, Return -1 And Back To Loop
	int bytesReceived = recv(m_Socket, buffer, sizeof(buffer) - 1, 0);

	if (bytesReceived > 0)
	{
		// Data Get Succesed
		// Need Data Update, And Ranking UI Logic
		Debug::D_Out << "[Network Manager] Received: " << buffer << std::endl;
	}
	else if (bytesReceived == 0)
	{
		// If Disconnect Server Succesful
		Debug::D_Out << "[Network Manager] Server Closed Connection." << std::endl;
		Disconnect();
	}
	else
	{
		// State "SOCKET_ERROR"
		int error = WSAGetLastError();
		// "WSAEWOULDBLOCK" Is Mean Server Have No Data Not Yet
		if (error != WSAEWOULDBLOCK)
		{
			Debug::D_Out << "[Network Manager] Recv Error Code: " << error << std::endl;
			Disconnect();
		}
	}
}