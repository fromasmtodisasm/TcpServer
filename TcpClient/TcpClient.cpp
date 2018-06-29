#include "TcpClient.h"

namespace tcp{
	namespace client {

		TcpClient::TcpClient(std::string addr, int port)
			: addr(addr), port(port)
		{

		}


		TcpClient::~TcpClient()
		{

		}
		void TcpClient::ProcessIncomming(fd_set & Set)
		{

		}
	}
}