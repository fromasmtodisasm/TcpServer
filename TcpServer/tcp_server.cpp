#ifdef WIN32
#define NOMINMAX
#endif
#include <iostream>
#include <string>
#include <algorithm>
#include "tcp_server.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;
namespace tcp_server
{
	TcpServer::TcpServer(string addr, int port, MessageRecived handler) : addr(addr), port(port), reciveHandler(handler)
	{
		ClientsCount = 0;
	}
	TcpServer::~TcpServer()
	{
		for (auto Client = Clients.begin();
			Client != Clients.end();
			Client++
			)

		{

			shutdown(*Client, SHUT_RDWR);
			close(*Client);
			cout << "Connection closed" << endl;
			//Clients.erase(Client);
			cout << "Client erased" << endl;
		}
		
		delete[] buffer;

		close(MasterSocket);
		cleanup();
		cout << "Server stopped" << endl;
	}

	bool TcpServer::Start()
	{
		if (init())
		{
			MasterSocket = socket(
				AF_INET /* IPv4 */,
				SOCK_STREAM /* TCP */,
				IPPROTO_TCP);
			if (MasterSocket < 0)
			{
				perror("socket");
				return false;
			}

			sockaddr_in SockAddr;
			SockAddr.sin_family = AF_INET;
			SockAddr.sin_port = htons(port);
			SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
			bind(MasterSocket, (sockaddr*)(&SockAddr), sizeof(SockAddr));

			set_nonblock(MasterSocket);
			listen(MasterSocket, SOMAXCONN);
			cout << "Server started on port [" << port << "]" << endl;
			buffer = new char[BufferSize];
			loop();
			return true;
		}

		return true;
	}
	bool TcpServer::loop()
	{
		//Clients = new SOCKET[(SocketCount * sizeof(int))];
		timeval time_out;
		time_out.tv_sec = 0; time_out.tv_usec = 50000; // 0.05 sec
		while (true)
		{

			fd_set Set;
			FD_ZERO(&Set);
			FD_SET(MasterSocket, &Set);
			for (auto Client = Clients.begin();
				Client != Clients.end();
				Client++
				)
			{
				FD_SET(*Client, &Set);
			}
			/* Find max number of client */
			int Max = /*std::max(MasterSocket,
							*std::max_element(
							Clients.begin(),
							Clients.end())
							);*/
				Clients.size();
								   /* Select sockets for read */
			int ready = select(
#ifdef WIN32
				1, 
#else
				Max + 1,
#endif
				&Set, NULL, NULL, &time_out);
			//cout << "Ready: [" << ready << "]" << endl;
			if (ready > 0)
			{
				for (auto Client = Clients.begin();
					Client != Clients.end();
					Client++
					)
				{
					if (FD_ISSET(*Client, &Set))
					{
						int byteRecived = recv(
							*Client,
							buffer,
							BufferSize,
#ifndef WIN32
							MSG_NOSIGNAL
#endif // !WIN32
							NULL
						);

						if ((byteRecived == 0) && (errno != EAGAIN))
						{
							shutdown(*Client, SHUT_RDWR);
							close(*Client);
							cout << "Connection closed" << endl;
							//Clients.erase(Client);
							
							if (Clients.erase(Client) == Clients.end())
							{
								cout << "Client erased" << endl;
								break;
							}
						}
						else if (byteRecived != 0)
						{
							cout << "Recived [" << byteRecived << "]" << endl;
							if (reciveHandler != NULL)
							{
								reciveHandler(this, *Client, 0/*string(buffer,0,byteRecived)*/);
							}
						}
					}
				}

				if (FD_ISSET(MasterSocket, &Set))
				{
					sockaddr addr;
					static socklen_t addrlen = sizeof(sockaddr);
					SOCKET NewClient = accept(MasterSocket, &addr, &addrlen);
					set_nonblock(NewClient);
					cout << "New client connected with fd[" << NewClient << "]" << endl;
					Clients.insert(NewClient);
				}
			}
			

			//cout << buffer << endl
		}
	}
	bool TcpServer::Send(SOCKET client, std::string msg)
	{
		cout << "Sending..." << endl;//<< "***************************" << endl <<msg<< endl << "***************************" << endl;
		send(client, msg.c_str(), msg.size() + 1, 0);
		return true;
	}
	int TcpServer::set_nonblock(int fd)
	{
#ifndef WIN32
		int flags;
#else
		u_long flags;
#endif
#if defined(O_NONBLOCK)
		if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
			flags = 0;
		return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
		flags = 1;
	#ifndef WIN32
		return ioctl(fd, FIONBIO, &flags);
	#else
		return ioctlsocket(fd, FIONBIO, &flags);
	#endif
#endif
	}
	inline void TcpServer::disconnect(SOCKET client, int how)
	{
		shutdown(client, how);
		close(client);
	}

	inline bool TcpServer::init()
	{
#ifdef WIN32
		WSADATA data;
		WORD ver MAKEWORD(2, 2);
		int wsInit = WSAStartup(ver, &data);
		return wsInit == 0;
#endif
		return true;
	}

	inline void TcpServer::cleanup()
	{
#ifdef WIN32
		WSACleanup();
#endif
	}

	SOCKET TcpServer::CreateSocket()
	{
		return SOCKET();
	}

}
