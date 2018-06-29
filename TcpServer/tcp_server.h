#include <set>
#include "Sockets.h"
namespace tcp {
	namespace server
	{
		using namespace tcp;
		class TcpServer;
		typedef void(*MessageRecived)(TcpServer *server, Sockets *client, std::string msg);

		class TcpServer
		{
			std::string addr;
			int port;
			Sockets MasterSocket;
			int SlaveSocket;
			char *buffer;
			MessageRecived reciveHandler;
			std::set<Sockets*> Clients;
			int ClientsCount;
			const int BufferSize = 8 * 1024;
			const int SocketCount = 100;
			const std::string Greating = "Welcom to awesome server!\r\n";
		public:
			TcpServer(std::string addr, int port, MessageRecived MessageHandler);
			~TcpServer();
			bool Start();
			std::set<Sockets*> &GetClients();
			void BroadCast(Sockets *from, std::string msg);
		private:
			bool Run();
			void ProcessIncomming(fd_set &Set);
			bool CreateSocket();

		};
	}
}