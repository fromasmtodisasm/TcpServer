#include <iostream>
#include <set>
#include "Sockets.h"

using namespace std;
namespace tcp {
	Sockets::Sockets()
	{
	}
	Sockets::~Sockets()
	{
	}
	bool Sockets::Send(SOCKET client, std::string msg) {
		cout << "Sending..." << endl;
		send(client, msg.c_str(), msg.size() + 1, 0);
		return true;
	}

	int Sockets::SetNonBlock(int fd) {
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
	void Sockets::Disconnect(SOCKET client, int how) {
		shutdown(client, how);
		close(client);
	}
	bool Sockets::Init() {
#ifdef WIN32
		WSADATA data;
		WORD ver MAKEWORD(2, 2);
		int wsInit = WSAStartup(ver, &data);
		return wsInit == 0;
#endif
		return true;
	}
	void Sockets::Cleanup() {
#ifdef WIN32
		WSACleanup();
#endif
	}
	int Sockets::Listen(SOCKET MasterSocket) {
		return listen(MasterSocket, MaxClients);
	}
	SOCKET Sockets::Socket() {
		return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	int Sockets::Recv(SOCKET Client, char *buffer, int BufferSize) {
		return recv(Client, buffer, BufferSize,
#ifndef WIN32
			MSG_NOSIGNAL
#else
			NULL
#endif // !WIN32
		);
	}
	int RecvAll(SOCKET Client, char *buffer, int BufferSize)
	{
		int len = 0;
	}
	SOCKET Sockets::Accept(SOCKET MasterSocket) {
		return accept(MasterSocket, NULL, NULL);
	}
	int Sockets::Select(fd_set &Set, timeval &time_out) {
		return select(
#ifdef WIN32
			1,
#else
			Clients.size() + 1,
#endif
			&Set, NULL, NULL, &time_out);
	}
	void Sockets::FillSet(SOCKET MasterSocket, fd_set &Set, std::set<SOCKET> Clients) {
		/* Install MasterSocket for select */
		FD_SET(MasterSocket, &Set);
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			/* Install ClientSocket for select */
			FD_SET(*Client, &Set);
		}
	}



}