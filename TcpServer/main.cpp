#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "tcp_server.h"
#include <winsock.h>

#include <cstdlib>  
#include <csignal>  

using namespace std;

bool MessageHandler(server::TcpServer *server, net::sockets::Socket *client, string msg);
void SignalHandler(int signal);
int exit_handler1(void)
{
	printf("Inside exit_handler1()!n");
	system("pause");
	return 0;
}

server::TcpServer *g_server;
int main(int argc, char **argv)
{
	_onexit(exit_handler1);
	//string addr = "192.168.0.12";
	string addr = "127.0.0.1";
	int port = 12345;
	/*****************************************************/
	typedef void(*SignalHandlerPointer)(int);

	SignalHandlerPointer previousHandler;
	//previousHandler = signal(SIGINT, SIG_IGN);
	/*****************************************************/
	//exit(EXIT_SUCCESS);
	server::TcpServer *server = g_server = new server::TcpServer(addr, port, MessageHandler);
	if (!server->Start())
	{
		cout << "Error while server start" << endl;
	}
	return 0;
}

bool MessageHandler(server::TcpServer *server, net::sockets::Socket *client, string msg)
{
	string content = msg;
	//bool res = true;
	if (msg != "exit") {
		int len = content.size();
		
		cout << "Content len = " << len << endl;
		string res = "HTTP/1.1 200 OK\nConnection: Keep-Alive\nContent-Length: " + to_string(len);
		res += "\n\n" + content;
		ostringstream ss;
		ss << "SOCKET #" << client->GetSocket() << ": " << msg << "\r\n";
		server->BroadCast(client, ss.str());
		return false;
	}
	return true;
}

void SignalHandler(int signal)
{
	if (signal == SIGTERM) {
		// abort signal handler code  
		//delete g_server;
		//exit(-1);
	}
	else {
		system("pause");
		// ...  
	}
}
