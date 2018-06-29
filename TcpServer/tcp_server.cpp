#ifdef WIN32
#define NOMINMAX
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "tcp_server.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;
namespace tcp {
	namespace server {
		TcpServer::TcpServer(string addr, int port, MessageRecived handler)
			: addr(addr), port(port), reciveHandler(handler)
		{
			ClientsCount = 0;
		}
		TcpServer::~TcpServer() {
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
				Disconnect(*Client, SHUT_RDWR);
			}
			delete[] buffer;
			close(MasterSocket);
			Cleanup();
			cout << "Server stopped" << endl;
		}
		bool TcpServer::Start() {
			if (Init()) {
				if (CreateSocket()) {
					cout << "Server started on port [" << port << "]" << endl;
					buffer = new char[BufferSize];
					Run();
					return true;
				}
				return false;
			}
			return true;
		}
		bool TcpServer::Run() {
			timeval time_out;
			time_out.tv_sec = 0; time_out.tv_usec = 50000; // 0.05 sec
			while (true) {
				fd_set Set;
				FD_ZERO(&Set);
				/* Fill socket set */
				FillSet(MasterSocket, Set, Clients);
				/* Select sockets for read */
				if (Select(Set, time_out) > 0) {
					ProcessIncomming(Set);
				}
			}
		}
		std::set<SOCKET> &TcpServer::GetClients() {
			return Clients;
		}
		void TcpServer::BroadCast(SOCKET from, string msg)
		{
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++)
			{
				if (from != *Client)
				{
					Send(*Client, msg);
				}
			}
		}

		void TcpServer::ProcessIncomming(fd_set &Set)
		{
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
				if (FD_ISSET(*Client, &Set)) {
					int byteRecived = Recv(*Client, buffer, BufferSize);
					if ((byteRecived <= 0) && (errno != EAGAIN)) {
						Disconnect(*Client, SHUT_RDWR);
						FD_CLR(*Client, &Set);
						cout << "Connection closed" << endl;

						ostringstream ss;
						ss << "<SOCKET #" << *Client << ": " << "Offline>\r\n";
						bool is_end = (Clients.erase(Client) == Clients.end());
						BroadCast(*Client, ss.str());
						if (is_end) {
							break;
						}
					}
					else {
						cout << "Recived [" << byteRecived << "]" << endl;
						if (reciveHandler != NULL) {
							reciveHandler(this, *Client, string(buffer, 0, byteRecived));
						}
						buffer[0] = '\0';
					}
				}
			}
			if (FD_ISSET(MasterSocket, &Set)) {
				SOCKET NewClient = Accept(MasterSocket);
				SetNonBlock(NewClient);
				cout << "New client connected with fd[" << NewClient << "]" << endl;
				Send(NewClient, Greating);
				ostringstream ss;
				ss << "<SOCKET #" << NewClient << ": " << "Online>\r\n";
				BroadCast(NewClient, ss.str());
				Clients.insert(NewClient);
			}
		}
		bool TcpServer::CreateSocket() {
			if ((MasterSocket = Socket()) >= 0) {
				sockaddr_in SockAddr;
				SockAddr.sin_family = AF_INET;
				SockAddr.sin_port = htons(port);
				SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
				bind(MasterSocket, (sockaddr*)(&SockAddr), sizeof(SockAddr));

				SetNonBlock(MasterSocket);
				if (Listen(MasterSocket) == 0) {
					return true;
				}
				else {
					perror("listen");
				}
			}
			else {
				perror("socket");
			}
			return false;
		}
	}
}