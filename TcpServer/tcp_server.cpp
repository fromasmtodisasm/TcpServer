#ifdef WIN32
#define NOMINMAX
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "tcp_server.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;
namespace tcp_server
{
	TcpServer::TcpServer(string addr, int port, MessageRecived handler)
		: addr(addr), port(port), reciveHandler(handler)
	{
		ClientsCount = 0;
	}
	TcpServer::~TcpServer()	{
		for (auto Client = Clients.begin();	Client != Clients.end(); Client++) {
			Disconnect(*Client, SHUT_RDWR);
		}		
		delete[] buffer;
		close(MasterSocket);
		Cleanup();
		cout << "Server stopped" << endl;
	}
	bool TcpServer::Start()	{
		if (Init()) { 
			if (CreateSocket()) {
				cout << "Server started on port [" << port << "]" << endl;
				buffer = new char[BufferSize];
				Run();
				return true;
			}
			return false;
		}
		return true;
	}
	bool TcpServer::Run() {
		timeval time_out;
		time_out.tv_sec = 0; time_out.tv_usec = 50000; // 0.05 sec
		while (true) {
			fd_set Set;
			FD_ZERO(&Set);
			/* Fill socket set */
			FillSet(Set);
			/* Select sockets for read */		
			if (Select(Set, time_out) > 0) {
				ProcessIncomming(Set);
			}
		}
	}
	bool TcpServer::Send(SOCKET client, std::string msg) {
		cout << "Sending..." << endl;
		send(client, msg.c_str(), msg.size() + 1, 0);
		return true;
	}
	std::set<SOCKET> &TcpServer::GetClients() {
		return Clients;
	}
	void TcpServer::BroadCast(SOCKET from, string msg)
	{
		for (auto Client = Clients.begin();	Client != Clients.end(); Client++)
		{
			if (from != *Client)
			{
				Send(*Client, msg);
			}
	}
}
	int TcpServer::SetNonBlock(int fd) {
#ifndef WIN32
		int flags;
#else
		u_long flags;
#endif
#if defined(O_NONBLOCK)
		if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
			flags = 0;
		return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
		flags = 1;
	#ifndef WIN32
		return ioctl(fd, FIONBIO, &flags);
	#else
		return ioctlsocket(fd, FIONBIO, &flags);
	#endif
#endif
	}
	inline void TcpServer::Disconnect(SOCKET client, int how) {
		shutdown(client, how);
		close(client);
	}
	inline bool TcpServer::Init() {
#ifdef WIN32
		WSADATA data;
		WORD ver MAKEWORD(2, 2);
		int wsInit = WSAStartup(ver, &data);
		return wsInit == 0;
#endif
		return true;
	}
	inline void TcpServer::Cleanup() {
#ifdef WIN32
		WSACleanup();
#endif
	}
	inline int TcpServer::Listen() {
		return listen(MasterSocket, MaxClients);
	}
	inline SOCKET TcpServer::Socket() {
		return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	inline int TcpServer::Recv(SOCKET Client) {
		return recv(Client, buffer, BufferSize,
#ifndef WIN32
			MSG_NOSIGNAL
#else
			NULL
#endif // !WIN32
		);
	}
	inline SOCKET TcpServer::Accept() {
		return accept(MasterSocket, NULL, NULL);
	}
	inline int TcpServer::Select(fd_set &Set, timeval &time_out) {
		return select(
#ifdef WIN32
			1,
#else
			Clients.size() + 1,
#endif
			&Set, NULL, NULL, &time_out);
	}
	inline void TcpServer::FillSet(fd_set &Set)	{
		/* Install MasterSocket for select */
		FD_SET(MasterSocket, &Set);
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			/* Install ClientSocket for select */
			FD_SET(*Client, &Set);
		}
	}
	void TcpServer::ProcessIncomming(fd_set &Set)
	{
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			if (FD_ISSET(*Client, &Set)) {
				int byteRecived = Recv(*Client);
				if ((byteRecived <= 0) && (errno != EAGAIN)) {
					Disconnect(*Client, SHUT_RDWR);
					FD_CLR(*Client, &Set);
					cout << "Connection closed" << endl;
					if (Clients.erase(Client) == Clients.end()) {
						break;
					}
				}
				else {
					cout << "Recived [" << byteRecived << "]" << endl;
					if (reciveHandler != NULL) {
						reciveHandler(this, *Client, string(buffer, 0, byteRecived));
					}
					buffer[0] = '\0';
				}
			}
		}
		if (FD_ISSET(MasterSocket, &Set)) {
			SOCKET NewClient = Accept();
			SetNonBlock(NewClient);
			cout << "New client connected with fd[" << NewClient << "]" << endl;
			Send(NewClient, Greating);
			ostringstream ss;
			ss << "<SOCKET #" << NewClient << ": " << "Online>\r\n";
			BroadCast(NewClient, ss.str());
			Clients.insert(NewClient);
		}
	}
	bool TcpServer::CreateSocket() {
		if ((MasterSocket = Socket()) >= 0) {
			sockaddr_in SockAddr;
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_port = htons(port);
			SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
			bind(MasterSocket, (sockaddr*)(&SockAddr), sizeof(SockAddr));

			SetNonBlock(MasterSocket);
			if (Listen() == 0) {
				return true;
			}
			else {
				perror("listen");
			}
		}
		else {
			perror("socket");
		}
		return false;
	}
}
