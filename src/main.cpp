//#include <iostream>
//using namespace std;

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
