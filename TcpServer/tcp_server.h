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
#endif // !_MSVCVER
	class TcpServer;
	typedef void(*MessageRecived)(TcpServer *server, int client, char* msg);

	class TcpServer
	{
		std::string addr;
		int port;
		SOCKET MasterSocket;
		int SlaveSocket;
		char *buffer;
		MessageRecived reciveHandler;
		std::set<SOCKET> Clients;
		//SOCKET *Clients;
		int ClientsCount;
		const int BufferSize = 8 * 1024;
		const int SocketCount = 100;
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
	private:
		bool loop();
		int set_nonblock(int fd);
		inline void disconnect(SOCKET client, int how);
		inline bool init();
		inline void cleanup();
		SOCKET CreateSocket();
	};
}
