#include "ZMQ_Demo_Client.h"

#include <iostream>

#include <Core/Core.h>

using namespace std;

ZMQ_Demo_Client::ZMQ_Demo_Client() : ZMQ_Demo_Networker(){
	sockPtrs.push_back(new DemoSocket("frontend", &context, ZMQ_REQ));
	sockPtrs.push_back(new DemoSocket("deliverer", &context, ZMQ_REQ));
	
	SetSocketID(sockPtrs[0]->socket, "Client 1");
	SetSocketID(sockPtrs[1]->socket, "Client 1");
	
	flags.push_back(new ReadyFlag("isFrontConnected"));
}

ZMQ_Demo_Client::~ZMQ_Demo_Client(){
	for (ReadyFlag* fPtr : flags)
		delete fPtr;
	
	for (DemoSocket *sPtr : sockPtrs){
		delete sPtr->socket;
		delete sPtr;
	}
}

void ZMQ_Demo_Client::ConnectFront(std::string conStr){
	flags[0]->val = BindByPtr(sockPtrs[0]->socket, &conStr);
}

void ZMQ_Demo_Client::DoWork(){
	if (this->isReadyToWork()){
		sockPtrs[0]->socket->setsockopt(ZMQ_IDENTITY, "Client_1", sizeof("Client_1"));
		
		SendMessage(sockPtrs[0]->socket, "Get connection");
		
		std::string repMsg = RecvMessage(sockPtrs[0]->socket);
		
		cout << "Received message from acceptor: " << repMsg << "\n";
		
		//BindByPtr(sockPtrs[1]->socket, &repMsg);
		
		ConnectPtrToAddr(sockPtrs[1]->socket, &repMsg);
		
		for (int i = 0; i < 10; i++){
		//while(true){
			SendMessage(sockPtrs[0]->socket, ("Do work for " + nwID));
			repMsg = RecvMessage(sockPtrs[0]->socket);
			cout << "second reply from acceptor: " << repMsg << "\n";
			
			if (repMsg.compare("Ok") == 0){
				cout << "Work is beign done\n";
				//If work is being done, connect to deliverer, to get work result
				SendMessage(sockPtrs[1]->socket, ("Get result for " + nwID));
				
				//Wait for result here
				std::string workResult = RecvMessage(sockPtrs[1]->socket);
				cout << nwID << " received work result: \"" << workResult << "\n";
				
				//Put result to log file!! Log file not developed yet!!
				ToLog(workResult);
			}else{
				cout << "Have some problems: " << repMsg << "\n";
			}
			cout << "\n";
			Upp::Sleep(1000);
		}
		
	}else{
		std::cout << "Client: is not rady for work:\n";
		for (ReadyFlag *fPtr : this->flags)
			std::cout << "    " << fPtr->name << " = " << fPtr->val << "\n";
	}
	
	cout << "\n***\n";
	cout << "Cient work is done!\n";
}