#include "ZMQ_Demo_Acceptor.h"

using namespace std;

ZMQ_Demo_Acceptor::ZMQ_Demo_Acceptor() : ZMQ_Demo_Networker()
{
	sockPtrs.push_back(new DemoSocket("frontend", &context, ZMQ_ROUTER));
	sockPtrs.push_back(new DemoSocket("backend", &context, ZMQ_DEALER));
	
	flags.push_back(new ReadyFlag("isFrontBind"));
	flags.push_back(new ReadyFlag("isBackConnected"));
}

ZMQ_Demo_Acceptor::~ZMQ_Demo_Acceptor(){
	for (ReadyFlag *fPtr : flags)
		delete fPtr;
	
	for (DemoSocket *sPtr : sockPtrs){
		delete sPtr->socket;
		delete sPtr;
	}
}

void ZMQ_Demo_Acceptor::BindFront(std::string bindStr){
	flags[0]->val = BindPtrToAddr(sockPtrs[0]->socket, &bindStr);
}

void ZMQ_Demo_Acceptor::ConnectBack(std::string conStr){
	flags[1]->val = ConnectPtrToAddr(sockPtrs[1]->socket, &conStr);
}

void ZMQ_Demo_Acceptor::DoWork()
{
	if (this->isReadyToWork()){
		cout << "Acceptor working\n";
		while (true) {
			string addr = RecvMessage(sockPtrs[0]->socket);
			RecvMessage(sockPtrs[0]->socket);
			string message = RecvMessage(sockPtrs[0]->socket);
			
			cout << "Have message: " << message << " from: " << addr << "\n";
			
			if (message == "Get connection"){
				cout << "Send back connection params\n";
				SendMessageWFlag(sockPtrs[0]->socket, addr, ZMQ_SNDMORE);
				SendMessageWFlag(sockPtrs[0]->socket, "", ZMQ_SNDMORE);
				SendMessage(sockPtrs[0]->socket, "tcp://localhost:5556");
			}else if (message.find("Do work for ") == 0){
				cout << "Request from client for doing work: " << message << "\n";
				SendMessageWFlag(sockPtrs[0]->socket, addr, ZMQ_SNDMORE);
				SendMessageWFlag(sockPtrs[0]->socket, "", ZMQ_SNDMORE);
				SendMessage(sockPtrs[0]->socket, "Ok");
				
				cout << "Send message:" << message << " from: " << addr << " to Worker main\n";
				SendMessageWFlag(sockPtrs[1]->socket, addr, ZMQ_SNDMORE);
				SendMessage(sockPtrs[1]->socket, "(acceptor)" + message);
			}else{
				cout << "unknown message on front: " << message << "\n";
			}
		}
	}else{
		cout << "Acceptor is not ready:\n";
		for (ReadyFlag *fPtr: flags){
			cout << fPtr->name << " = " << fPtr->val << "\n";
		}
	}
}