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
				(*Client)->Disconnect(Sockets::SHUT_RDWR);
			}
			delete[] buffer;
			MasterSocket.Close();
			MasterSocket.Cleanup();
			cout << "Server stopped" << endl;
		}
		bool TcpServer::Start() {
			if (MasterSocket.Init()) {
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
				MasterSocket.FillSet(Set, Clients);
				/* Select sockets for read */
				if (MasterSocket.Select(Set, time_out) > 0) {
					ProcessIncomming(Set);
				}
			}
		}
		std::set<Sockets*> &TcpServer::GetClients() {
			return Clients;
		}
		void TcpServer::BroadCast(Sockets *from, string msg)
		{
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++)
			{
				if (from != *Client)
				{
					(*Client)->Send(msg);
				}
			}
		}

		void TcpServer::ProcessIncomming(fd_set &Set)
		{
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
				if (FD_ISSET((*Client)->GetSocket(), &Set)) {
					int byteRecived = (*Client)->Recv(buffer, BufferSize);
					if ((byteRecived <= 0) && (errno != EAGAIN)) {
						(*Client)->Disconnect(Sockets::SHUT_RDWR);
						FD_CLR((*Client)->GetSocket(), &Set);
						cout << "Connection closed" << endl;

						ostringstream ss;
						ss << "<SOCKET #" << *Client << ": " << "Offline>\r\n";
						
						BroadCast(*Client, ss.str());
						delete *Client;
						bool is_end = (Clients.erase(Client) == Clients.end());
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
			if (FD_ISSET(MasterSocket.GetSocket(), &Set)) {
				Sockets *NewClient = MasterSocket.Accept();
				NewClient->SetNonBlock();
				cout << "New client connected with fd[" << NewClient->GetSocket() << "]" << endl;
				NewClient->Send(Greating);
				ostringstream ss;
				ss << "<SOCKET #" << NewClient << ": " << "Online>\r\n";
				BroadCast(NewClient,ss.str());
				Clients.insert(NewClient);
			}
		}
		bool TcpServer::CreateSocket() {
			if ((MasterSocket.Socket()) >= 0) {
				sockaddr_in SockAddr;
				SockAddr.sin_family = AF_INET;
				SockAddr.sin_port = htons(port);
				SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
				MasterSocket.Bind((sockaddr*)(&SockAddr), sizeof(SockAddr));

				MasterSocket.SetNonBlock();
				if (MasterSocket.Listen() == 0) {
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