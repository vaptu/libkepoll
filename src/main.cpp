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
	string str("127.0.0.1");
	My l(str, 1234);
	l.Start();
}
