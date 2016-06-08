# libepoll
libepoll使用epoll基础模型为基础。分离socket与epol具体逻辑。只需关心具体的socket处理流程
```c++
#include <iostream>
using namespace std;

#include "listen.h"

class My : public Listen{
	public:
		My(string host, int port):Listen(host,port){}

		string & Handle(string &data){
			return data;
		}
};

int main(){
	My l(string("127.0.0.1"), 1234);
	l.Start();
}
```
