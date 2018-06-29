#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "tcp_server.h"

using namespace tcp::server;
using namespace std;

void MessageHandler(TcpServer *server, Sockets *client, string msg);

int main(int argc, char **argv)
{
	string addr = "192.168.0.12";
	int port = 12345;

	TcpServer server(addr, port, MessageHandler);
	if (!server.Start())
	{
		cout << "Error on server start" << endl;
	}
	return 0;
}

void MessageHandler(TcpServer *server, Sockets *client, string msg)
{
	string content = msg;
	int len = content.size();
	cout << "Content len = " << len << endl;
	string res;// = "HTTP/1.1 200 OK\nConnection: Keep-Alive\nContent-Length: " + to_string(len);
	res += "\n\n" + content;
	auto Clients = server->GetClients();
	ostringstream ss;
	ss << "SOCKET #" << client->GetSocket() << ": " << msg << "\r\n";
	server->BroadCast(client, ss.str());
}
