
//This part is for testing purposes.
//Not for publishing
/*
#include "ZMQ_Worker.h"


#include <iostream>

//To get upp threads
#include <Core/Core.h>

using namespace std;

//Try to figure out why does not SetSocketID function does not work properly

void SetSockPortTest(){
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_ROUTER);
	
	if (BindPtrToAddr(&socket, "tcp://*:5555")) {
		cout << "router socket bind\n";
		
		string addr = RecvMessage(&socket);
		RecvMessage(&socket);
		string message = RecvMessage(&socket);
		
		cout << "Have message: " << message << " from: " << addr << "\n";
		
	}else{
		cout << "router socket is not bind\n";
	}
	
	
}


int main(){
	
	cout << "Testing SetSocketID function\n";
	
	zmq::context_t context(1);
	
	zmq::socket_t socket(context, ZMQ_REQ);
	
	//socket.setsockopt(ZMQ_IDENTITY, "Test Socket");
	
	SetSocketID(&socket, "test_socket_id");
	
	Upp::Thread *tPtr = new Upp::Thread();
	
	tPtr->Run([] {SetSockPortTest();});
	
	Upp::Sleep(100);
	
	socket.connect("tcp://localhost:5555");
	
	SendMessage(&socket, "Test message");
	
	while(Upp::Thread().GetCount() != 0){
		Upp::Sleep(100);
	}
	
	delete tPtr;
	
	cout << "Testing SetSocketID function END\n";
	
	return 1;
}*/