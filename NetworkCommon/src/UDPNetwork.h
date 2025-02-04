#pragma once

#include "winimports.h"

#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

constexpr u_short SERVER_PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

class UDPNetwork
{
public:
	UDPNetwork();
	~UDPNetwork();

	bool Init();
	void CleanUp();

	bool CreateSocket();
	bool BindSocket(u_short port);
	bool SendTo(const char* address, u_short port, const char* data, int dataSize);
	int ReceiveFrom(char* buffer, int bufferSize, sockaddr_in& senderAddr);

	u_short GetLocalPort() const { return m_localPort; }

	void StartListening();
	void StopListening();

private:
	void Listen();
	void Interpret();

private:
	// Socket
	SOCKET m_socket;
	u_short m_localPort;

	// Message Queue
	std::queue<std::vector<char>> m_messageQueue;
	std::mutex m_messageQueueMutex;
	std::condition_variable m_messageQueueCondition;
	
	// Listening
	std::thread m_listenThread;
	std::atomic_bool m_running;

	// Interpreter
	std::thread m_interpreterThread;
	std::atomic_bool m_processing;
};
