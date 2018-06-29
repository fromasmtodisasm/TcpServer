#pragma once
#include "Sockets.h"
namespace tcp {
	namespace client {
		class TcpClient
		{
			std::string addr;
			int port;
			SOCKET MySocket;
			char *buffer;
			const int BufferSize = 8 * 1024;
			const int SocketCount = 100;
			const std::string Greating = "Welcom to awesome server!\r\n";
		public:
			TcpClient(std::string addr, int port);;
			~TcpClient();
			bool Start();
		private:
			void ProcessIncomming(fd_set &Set);
			bool CreateSocket();
		};
	}
}