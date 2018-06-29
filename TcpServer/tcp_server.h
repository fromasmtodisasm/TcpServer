#include <set>
#include "Sockets.h"
namespace tcp {
	namespace server
	{
		using namespace tcp;
		class TcpServer;
		typedef void(*MessageRecived)(TcpServer *server, int client, std::string msg);

		class TcpServer : public Sockets
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
			const std::string Greating = "Welcom to awesome server!\r\n";
		public:
			TcpServer(std::string addr, int port, MessageRecived MessageHandler);
			~TcpServer();
			bool Start();
			std::set<SOCKET> &GetClients();
			void BroadCast(SOCKET from, std::string msg);
		private:
			bool Run();
			void ProcessIncomming(fd_set &Set);
			bool CreateSocket();

		};
	}
}