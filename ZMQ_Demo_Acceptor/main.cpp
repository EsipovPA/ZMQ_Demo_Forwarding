#include "ZMQ_Demo_Acceptor.h"

int main(){
	
	ZMQ_Demo_Acceptor testAc;
	
	testAc.BindFront("tcp://*:5555");
	testAc.ConnectBack("tcp://localhost:5557");
	
	testAc.DoWork();
	
	return 1;
}