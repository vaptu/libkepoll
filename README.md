# libkepoll
Libkepoll use epoll model-based foundation. Separate socket and epol specific logic. Only care about the specific socket processing.
Visit the [project's home(libkepoll)](http://kepoll.vaptu.com/) page and download the open source project
```c++
#include "server_kepoll.h"
#include <iostream>
using namespace std;



class MyServer : public ServerKEpoll{
	public:
	MyServer(const string addr, int port):ServerKEpoll(addr, port)
	{
	}
		//standard input 
	bool StdinHandle(int fd){
		char buf[2048] = {0};
		int re = read(fd, buf, 2048);
		if(re <=0)
			return false;

		buf[re] = '\0';
		cout << "stdin : " << buf <<endl;
		return true;
	}

		//client input 
	bool ClientHandle(int fd){
		char buf[2048] = {0};
		int re = read(fd, buf, 2048);
		if(re <=0)
			return false;

		buf[re] = '\0';
		cout << "client : " << buf <<endl;
		return true;

	}
		
        //administrator input
	bool OperaHandle(int fd){
		char buf[2048] = {0};
		int re = read(fd, buf, 2048);
		if(re <=0)
			return false;

		buf[re] = '\0';
		cout << "opera : " << buf <<endl;
		return true;
	}
};

int main()
{
	MyServer server("0.0.0.0", 4455);
    //Capture standard input
	server.SetStdinCatch();
    
    //Administrator listening port
	server.SetAdminer("0.0.0.0", 9999);
    
    //Multi-client port monitoring
	server.SetListener("0.0.0.0", 4444);
    
    //Heartbeat packet transmission interval
	server.SetHeartbeat(1000);

	//Start the message loop
	server.KEpoll_Loop();
}

```
