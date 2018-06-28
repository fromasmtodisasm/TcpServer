#pragma once
#include <iostream>
#include <string>
#include "tcp_server.h"

using namespace tcp_server;
using namespace std;

void MessageHandler(TcpServer *server, int client, char *msg);

int main(int argc, char **argv)
{
	string addr = "127.0.0.1";
	int port = 12345;

	TcpServer server(addr, port, MessageHandler);
	if (!server.Start())
	{
		cout << "Error on server start" << endl;
	}
	return 0;
}

void MessageHandler(TcpServer *server, int client, char *msg)
{
	string content = "\n\n<html>"\
		"<body>"\
		"<h1>Hello, World!</h1>"\
		"</body>"\
		"</html>";
	string res = "HTTP/1.1 200 OK\nConnection: Keep-Alive\nContent-Length: " + to_string(content.size() - 2);
	res += content;
	//cout << "Response:" << endl << res << endl << endl;;

	server->Send(client, res);
}
