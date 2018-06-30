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

#ifndef SOCKETSTATIC
#ifdef SOCKETSDLL_EXPORT
#define SOCKETSDLL_API __declspec(dllexport)
#else
#define SOCKETSDLL_API __declspec(dllimport)
#endif // SOCKETSDLL_EXPORT
#else
#define SOCKETSDLL_API
#endif

#include "AddressFamily.h"
#include "SocketType.h"
#include "ProtocolType.h"
namespace net {
	namespace sockets {
		class Socket
		{
		private:
			const int MaxClients = SOMAXCONN;
		public:
			SOCKETSDLL_API Socket(SOCKET socket);
			SOCKETSDLL_API Socket(
				AddressFamily::AddressFamily addressFamily,
				SocketType::SocketType socketType,
				ProtocolType::ProtocolType protocolType
			);
			SOCKETSDLL_API ~Socket();
			static enum ShutdownSocket
			{
#ifdef WIN32
				SHUT_RD = SD_RECEIVE,
				SHUT_WR = SD_SEND,
				SHUT_RDWR = SD_BOTH
#endif
			}ShutdownSocket;
		protected:
			SOCKET socket;
			sockaddr_in adr;
			//bool inited = false;
		public:
			SOCKETSDLL_API bool Send(std::string msg);
			SOCKETSDLL_API int SetNonBlock();
			SOCKETSDLL_API void Disconnect(int how);
			SOCKETSDLL_API bool Init();
			SOCKETSDLL_API void Cleanup();
			SOCKETSDLL_API int Bind(int port, std::string addres);
			SOCKETSDLL_API int Listen();
			SOCKETSDLL_API int Connect(int port, std::string addres);
			SOCKETSDLL_API void Close();
			SOCKETSDLL_API int Recive(char *buffer, int BufferSize);
			SOCKETSDLL_API int RecvAll(SOCKET Client, char *buffer, int BufferSize);
			SOCKETSDLL_API Socket *Accept();
			SOCKETSDLL_API int	Select(fd_set &Set, timeval &time_out);
			SOCKETSDLL_API void FillSet(fd_set &Set, std::set<Socket*> Clients);
			SOCKETSDLL_API SOCKET GetSocket();
		};
	}
}
