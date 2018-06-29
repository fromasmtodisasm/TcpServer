#pragma once
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 		/* close */
#include <arpa/inet.h>	/* inet_addr */
#include <fcntl.h>			/* fcntl */
typedef int SOCKET;
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#define close closesocket
#endif // !WIN32

#include <string>
#include <set>
namespace tcp {

	class Sockets
	{
	private:
		const int MaxClients = SOMAXCONN;
	public:
		Sockets(SOCKET socket);
		Sockets();
		~Sockets();
		static enum ShutdownSocket
		{
#ifdef WIN32
			SHUT_RD = SD_RECEIVE,
			SHUT_WR = SD_SEND,
			SHUT_RDWR = SD_BOTH
#endif
		}ShutdownSocket;
	protected:

		SOCKET _Socket;
	public:
		bool Send(std::string msg);
		int SetNonBlock();
		void Disconnect(int how);
		bool Init();
		void Cleanup();
		int Bind(const struct sockaddr * name, int namelen);
		int  Listen();
		bool Socket();
		void Close();
		int Recv(char *buffer, int BufferSize);
		int RecvAll(char *buffer, int BufferSize);
		Sockets *Accept();
		int	Select(fd_set &Set, timeval &time_out);
		void FillSet(fd_set &Set, std::set<Sockets*> Clients);
		SOCKET GetSocket();
	};

}
