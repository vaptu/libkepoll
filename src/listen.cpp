#include "listen.h"


Listen::Listen(){

}

Listen::Listen(string host, int port){
	this->host = host;
	this->port = port;
}

Listen::~Listen(){

}

bool Listen::Start(){
	if((this->listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket create failed!");
		return false;
	}
	int flag = 1, len=sizeof(int);
	setsockopt(this->listen_socket, SOL_SOCKET, SO_REUSEADDR, &flag, len);
	if(this->setnonblocking(this->listen_socket) == false){
		perror("set nonblock error");
		return false;
	}

	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(this->port);
	addr.sin_addr.s_addr=inet_addr(this->host.c_str());

	if(bind(this->listen_socket, (struct sockaddr*)&addr, sizeof(sockaddr)) == -1){
		perror("cannot bind the socket!");
		return false;
	}

	if(listen(this->listen_socket, this->MAXBAK) == -1){
		perror("listen failed!");
		return false;
	}

	struct epoll_event ev;
	struct epoll_event events[10000];
	int kdpfd = epoll_create(10000);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = this->listen_socket;
	if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, this->listen_socket, &ev) <0 ){
		perror("epoll set insertion error");
		return false;
	}
		
	struct sockaddr_in addr_tmp;
	socklen_t socklen = sizeof(sockaddr);
	int nfds=0, curfds=1;
	int connfd=0;

	while(true){
		nfds = epoll_wait(kdpfd, events, curfds, -1);
		if(nfds == -1){
			perror("epoll_wait");
			continue;
		}

		for(int n=0; n<nfds; ++n){
			if( events[n].data.fd == this->listen_socket){
				if((connfd = accept(this->listen_socket, (sockaddr *)&addr_tmp, &socklen)) == -1){
					perror("accept falied");
					return -1;
				}
					//accept success
				client_info info{addr_tmp.sin_port, string(inet_ntoa(addr_tmp.sin_addr))};
				this->Connected(&info);
				//printf("connected from %s:%d\n", inet_ntoa(addr_tmp.sin_addr), addr_tmp.sin_port);

				client_info info_tmp;
				info_tmp.port = addr_tmp.sin_port;
				info_tmp.host = inet_ntoa(addr_tmp.sin_addr);
				this->client_list.insert(make_pair(connfd, info_tmp));

				if(curfds >= 10000){
					cerr << " too many connection" <<endl;
					close(connfd);
					return -1;
				}
				if(this->setnonblocking(connfd) == false){
					perror("set accept nonblocking failed");
					close(connfd);
					return -1;
				}
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, connfd, &ev)<0){
					perror("add socket to epoll list failed");
					close(connfd);
					return -1;
				}
				curfds++;
				continue;
			}

			// deal with logtic
			if(this->before(events[n].data.fd) <0){
					//delete list
				client_info client = this->client_list[events[n].data.fd];
				this->client_list.erase(this->client_list.find(events[n].data.fd));

					//delete epoll list
				epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
				curfds--;

					//callbak function
				this->Disconnected(&client);
			}
		}
	}
	close(this->listen_socket);
	return true;
}

bool Listen::setnonblocking(int sockfd){
	if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1){
		return false;
	}
	return true;
}

string & Listen::Handle(string &data){};

int Listen::before(int connfd){
	int nread=0;
	char buf[this->MAXBUF];
	nread = read(connfd,buf, this->MAXBUF);
	if(nread <= 0){
		close(connfd);
		return -1;
	}
	string data(buf);
	string result = this->Handle(data);
		// send
	write(connfd, result.c_str(), result.length());

	return 0;
}

void Listen::Connected(const client_info *info){}

void Listen::Disconnected(const client_info *info){}
	
