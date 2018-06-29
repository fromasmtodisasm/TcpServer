#include <set>
namespace tcp_server
{
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 		/* close */
#include <arpa/inet.h>	/* inet_addr */
#include <fcntl.h>			/* fcntl */
	typedef int SOCKET
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#define close closesocket
#endif // !WIN32
	class TcpServer;
	typedef void(*MessageRecived)(TcpServer *server, int client, std::string msg);

	class TcpServer
	{
		std::string addr;
		int port;
		SOCKET MasterSocket;
		int SlaveSocket;
		char *buffer;
		MessageRecived reciveHandler;
		std::set<SOCKET> Clients;
		int ClientsCount;
		const int BufferSize = 8 * 1024;
		const int SocketCount = 100;
		const int MaxClients = SOMAXCONN;
		const std::string Greating = "Welcom to awesome server!\r\n";
		enum ShutdownSocket
		{
#ifdef WIN32
			SHUT_RD = SD_RECEIVE,
			SHUT_WR = SD_SEND,
			SHUT_RDWR = SD_BOTH
#endif
		}ShutdownSocket;
	public:
		TcpServer(std::string addr, int port, MessageRecived MessageHandler);
		~TcpServer();
		bool Start();
		bool Send(SOCKET client, std::string msg);
		std::set<SOCKET> &GetClients();
		void BroadCast(SOCKET from, std::string msg);
	private:
		bool Run();
		int SetNonBlock(int fd);
		inline void Disconnect(SOCKET client, int how);
		inline bool Init();
		inline void Cleanup();
		inline int  Listen();
		inline SOCKET Socket();
		inline int Recv(SOCKET Client);
		inline SOCKET Accept();
		inline int	Select(fd_set &Set, timeval &time_out);
		inline void FillSet(fd_set &Set);
		void ProcessIncomming(fd_set &Set);
		bool CreateSocket();
		
	};
}
