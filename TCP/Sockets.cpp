#include <iostream>
#include <set>
#include "Sockets.h"

using namespace std;
namespace tcp {
	Sockets::Sockets()
	{

	}
	Sockets::Sockets(SOCKET socket) : _Socket(socket)
	{

	}
	Sockets::~Sockets()
	{
	}
	bool Sockets::Send(std::string msg) {
		cout << "Sending..." << endl;
		send(this->_Socket, msg.c_str(), msg.size() + 1, 0);
		return true;
	}

	int Sockets::SetNonBlock() {
#ifndef WIN32
		int flags;
#else
		u_long flags;
#endif
#if defined(O_NONBLOCK)
		if ((flags = fcntl(this->_Socket, F_GETFL, 0)) == -1)
			flags = 0;
		return fcntl(this->_Socket, F_SETFL, flags | O_NONBLOCK);
#else
		flags = 1;
#ifndef WIN32
		return ioctl(this->_Socket, FIONBIO, &flags);
#else
		return ioctlsocket(this->_Socket, FIONBIO, &flags);
#endif
#endif
	}
	void Sockets::Disconnect(int how) {
		shutdown(this->_Socket, how);
		close(this->_Socket);
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
	int Sockets::Bind(const sockaddr * name, int namelen)
	{
		return bind(this->_Socket, name, namelen);
	}
	int Sockets::Listen() {
		return listen(this->_Socket, MaxClients);
	}
	bool Sockets::Socket() {
		this->_Socket =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		return this->_Socket >= 0;
	}
	void Sockets::Close()
	{
		close(this->_Socket);
	}
	int Sockets::Recv(char *buffer, int BufferSize) {
		return recv(this->_Socket, buffer, BufferSize,
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
		do
		{

		} while (true);
	}
	Sockets *Sockets::Accept() {
		Sockets *socket =  new Sockets(accept(this->_Socket, NULL, NULL));
		return socket;
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
	void Sockets::FillSet(fd_set &Set, std::set<Sockets*> Clients) {
		/* Install MasterSocket for select */
		FD_SET(this->_Socket, &Set);
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			/* Install ClientSocket for select */
			FD_SET((*Client)->GetSocket(), &Set);
		}
	}

	SOCKET Sockets::GetSocket()
	{
		return _Socket;
	}



}