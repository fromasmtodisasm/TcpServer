#pragma once
namespace tcp {
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
	class Sockets
	{
	private:
		const int MaxClients = SOMAXCONN;
	public:
		Sockets();
		~Sockets();

	protected:
		enum ShutdownSocket
		{
#ifdef WIN32
			SHUT_RD = SD_RECEIVE,
			SHUT_WR = SD_SEND,
			SHUT_RDWR = SD_BOTH
#endif
		}ShutdownSocket;
	protected:
		bool Send(SOCKET client, std::string msg);
		int SetNonBlock(int fd);
		void Disconnect(SOCKET client, int how);
		bool Init();
		void Cleanup();
		int  Listen(SOCKET MasterSocket);
		SOCKET Socket();
		int Recv(SOCKET Client, char *buffer, int BufferSize);
		SOCKET Accept(SOCKET MasterSocket);
		int	Select(fd_set &Set, timeval &time_out);
		void FillSet(SOCKET MasterSocket, fd_set &Set, std::set<SOCKET> Clients);
	};

}
