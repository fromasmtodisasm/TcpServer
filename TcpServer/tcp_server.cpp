#ifdef WIN32
#define NOMINMAX
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "tcp_server.h"
#include "SocketException.h"
#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;
using namespace net::sockets;

namespace server {
	TcpServer::TcpServer(string addr, int port, MessageRecived handler)
		: addr(addr), port(port), reciveHandler(handler)
	{
		ClientsCount = 0;
	}
	TcpServer::~TcpServer() {
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			(*Client).first->Send("Server stopped");
			(*Client).first->Disconnect(Socket::SHUT_RDWR);
			delete (*Client).first;
		}
		delete[] buffer;
		delete MasterSocket;
		cout << "Server stopped" << endl;
	}
	bool TcpServer::Start() {
		if (CreateSocket()) {
			cout << "Server started on port [" << port << "]" << endl;
			buffer = new char[BufferSize];
			Run();
			return true;
		}
		return false;
	}
	bool TcpServer::Run() {
		timeval time_out;
		time_out.tv_sec = 0; time_out.tv_usec = 50000; // 0.05 sec
		while (!is_exit) {
			fd_set Set;
			FD_ZERO(&Set);
			/* Fill socket set */
			/* Install MasterSocket for select */
			FD_SET(MasterSocket->GetSocket(), &Set);
			for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
				/* Install ClientSocket for select */
				FD_SET((*Client).first->GetSocket(), &Set);
			}
			/* Select sockets for read */
			if (MasterSocket->Select(Set, time_out) > 0) {
				ProcessIncomming(Set);
			}
		}
		return true;
	}
	std::map<Socket*, Client*> &TcpServer::GetClients() {
		return Clients;
	}
	void TcpServer::BroadCast(Socket *from, string msg) {
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			if (from != (*Client).first) {
				(*Client).first->Send(msg);
			}
		}
	}
	void TcpServer::ProcessIncomming(fd_set &Set)
	{
		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			if (FD_ISSET((*Client).first->GetSocket(), &Set)) {
				int byteRecived = (*Client).first->Recive(buffer, BufferSize);
				if ((byteRecived <= 0) && (errno != EAGAIN)) {
					(*Client).first->Disconnect(Socket::SHUT_RDWR);
					FD_CLR((*Client).first->GetSocket(), &Set);
					cout << "Connection closed" << endl;

					ostringstream ss;
					ss << "<SOCKET #" << (*Client).first->GetSocket() << ": " << "Offline>\r\n";
						
					BroadCast((*Client).first, ss.str());
					delete (*Client).first;
					delete (*Client).second;
					ClientsCount--;
					cout << "Clients: " << ClientsCount << endl;
					if (Clients.erase(Client) == Clients.end()) {
						break;
					}
				}
				else {
					cout << "Recived [" << byteRecived << "]" << endl;
					if (reciveHandler != NULL) {
						is_exit = reciveHandler(this, (*Client).first, string(buffer, 0, byteRecived));
					}
					buffer[0] = '\0';
				}
			}
		}
		if (FD_ISSET(MasterSocket->GetSocket(), &Set)) {
			Socket *NewSocket = MasterSocket->Accept();
			Client *NewClient = new Client();
			NewSocket->SetNonBlock();
			cout << "New client connected with fd[" << NewSocket->GetSocket() << "]" << endl;
			NewSocket->Send(Greating);
			ostringstream ss;
			ss << "<SOCKET #" << NewSocket->GetSocket() << ": " << "Online>\r\n";
			BroadCast(NewSocket,ss.str());
			Clients[NewSocket] = NewClient;
			ClientsCount++;
			cout << "Clients: " << ClientsCount << endl;
		}
	}
	bool TcpServer::CreateSocket() {
		try {
			MasterSocket = new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
			MasterSocket->Bind(port, addr.c_str());

			MasterSocket->SetNonBlock();
			if (MasterSocket->Listen() == 0) {
				return true;
			}
			else {
				cerr << "Error on Listen" << endl;
			}
		}
		catch(SocketException se) {
			cerr << se.what() << endl;
		}
		return false;
	}
}
