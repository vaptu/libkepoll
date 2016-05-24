# libepoll
libepoll使用epoll基础模型为基础。分离socket与epol具体逻辑。只需关心具体的socket处理流程
```c++
#include <iostream>
using namespace std;

#include "listen.h"

class My : public Listen{
	public:
		int Handle(int connfd){
			char buf[2048];
			read(connfd,buf,2048);
			cout << buf << endl;
		}
};

int main(){ 
	My s;
	s.Start();
}
```
