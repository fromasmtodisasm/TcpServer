#include <iostream>
#include <set>
#include "Sockets.h"
#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;
namespace net {
	namespace sockets {
		Socket::Socket(
			AddressFamily::AddressFamily addressFamily,
			SocketType::SocketType socketType,
			ProtocolType::ProtocolType protocolType
		)
		{
			socket = ::socket(addressFamily, socketType, protocolType);
			if (socket == INVALID_SOCKET)
			{
				throw false;
			}
		}
		Socket::Socket(SOCKET socket) : socket(socket)
		{

		}
		Socket::~Socket()
		{
		}
		bool Socket::Send(std::string msg) {
			cout << "Sending..." << endl;
			::send(socket, msg.c_str(), msg.size() + 1, 0);
			return true;
		}

		int Socket::SetNonBlock() {
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
			return ioctlsocket(socket, FIONBIO, &flags);
#endif
#endif
		}
		void Socket::Disconnect(int how) {
			::shutdown(socket, how);
			close(socket);
		}
		bool Socket::Init() {
#ifdef WIN32
			int wsInit;
			WSADATA data;
			WORD ver MAKEWORD(2, 2);
			wsInit = WSAStartup(ver, &data);
			return (wsInit == 0);
#endif
			return true;
		}
		void Socket::Cleanup() {
#ifdef WIN32
			WSACleanup();
#endif
		}
		int Socket::Bind(int port, std::string addres) {
			sockaddr_in SockAddr;
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_port = htons(port);
			SockAddr.sin_addr.s_addr = inet_addr(addres.c_str());
			return bind(socket, (sockaddr*)&SockAddr, sizeof(SockAddr));
		}
		int Socket::Listen() {
			return ::listen(socket, MaxClients);
		}
		int Socket::Connect(int port, std::string addres)
		{
			sockaddr_in SockAddr;
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_port = htons(port);
			SockAddr.sin_addr.s_addr = inet_addr(addres.c_str());
			return ::connect(socket, (sockaddr*)&SockAddr, sizeof(SockAddr));
		}
		void Socket::Close() {
			close(socket);
		}
		int Socket::Recive(char *buffer, int BufferSize) {
			return recv(socket, buffer, BufferSize,
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
		Socket *Socket::Accept() {
			Socket *accepted = new Socket(accept(socket, NULL, NULL));
			return accepted;
		}
		int Socket::Select(fd_set &Set, timeval &time_out) {
			return ::select(
#ifdef WIN32
				1,
#else
				Clients.size() + 1,
#endif
				&Set, NULL, NULL, &time_out);
		}
		void Socket::FillSet(fd_set &Set, std::set<Socket*> Clients) {
			/* Install MasterSocket for select */
			FD_SET(this->socket, &Set);
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
				/* Install ClientSocket for select */
				FD_SET((*Client)->GetSocket(), &Set);
			}
		}
		SOCKET Socket::GetSocket() {
			return socket;
		}
	}
}