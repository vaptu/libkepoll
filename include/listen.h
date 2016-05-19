#ifndef LISTEN_H
#define LISTEN_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct client_info{
	int port;
	string host;
};

class Listen{
	public:
		Listen();
		Listen(string host, int port);
		~Listen();

		bool Start();
		virtual int Handle(int connfd);

	protected:
		int MAXBUF = 2048;
		int MAXBAK = 5;

		int 	port=5555;
		string 	host="0.0.0.0";
		int		listen_socket=0;

		map<int, client_info>	client_list;

		bool setnonblocking(int sockfd);
};


#endif
