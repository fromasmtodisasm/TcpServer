#include <map>
#include "Sockets.h"
#include "Client.h"
namespace server
{
	class TcpServer;
	typedef void(*MessageRecived)(TcpServer *server, net::sockets::Socket *client, std::string msg);

	class TcpServer
	{
		std::string addr;
		int port;
		char *buffer;
		net::sockets::Socket *MasterSocket;
		std::map<net::sockets::Socket*,Client*> Clients;
		MessageRecived reciveHandler;
		int ClientsCount;
		const int BufferSize = 8 * 1024;
		const int SocketCount = 100;
		const std::string Greating = "Welcom to awesome server!\r\n";
	public:
		TcpServer(std::string addr, int port, MessageRecived MessageHandler);
		~TcpServer();
		bool Start();
		std::map<net::sockets::Socket*, Client*> &GetClients();
		void BroadCast(net::sockets::Socket *from, std::string msg);
	private:
		bool Run();
		void ProcessIncomming(fd_set &Set);
		bool CreateSocket();

	};
}
