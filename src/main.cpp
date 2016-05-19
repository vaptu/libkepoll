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
