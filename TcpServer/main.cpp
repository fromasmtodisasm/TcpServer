#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "tcp_server.h"

using namespace std;

void MessageHandler(server::TcpServer *server, net::sockets::Socket *client, string msg);

int main(int argc, char **argv)
{
	//string addr = "192.168.0.12";
	string addr = "127.0.0.1";
	int port = 12345;

	server::TcpServer server(addr, port, MessageHandler);
	if (!server.Start())
	{
		cout << "Error while server start" << endl;
	}
	return 0;
}

void MessageHandler(server::TcpServer *server, net::sockets::Socket *client, string msg)
{
	string content = msg;
	int len = content.size();
	cout << "Content len = " << len << endl;
	string res = "HTTP/1.1 200 OK\nConnection: Keep-Alive\nContent-Length: " + to_string(len);
	res += "\n\n" + content;
	ostringstream ss;
	ss << "SOCKET #" << client->GetSocket() << ": " << msg << "\r\n";
	server->BroadCast(client, ss.str());
}
