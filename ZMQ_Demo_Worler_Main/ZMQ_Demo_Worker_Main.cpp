#include "ZMQ_Demo_Worker_Main.h"

void WorkerSubThread(string workerID);

ZMQ_Demo_Worker_Main::ZMQ_Demo_Worker_Main() : ZMQ_Demo_Networker(){
	sockPtrs.push_back(new DemoSocket("frontend", &context, ZMQ_ROUTER));
	sockPtrs.push_back(new DemoSocket("backend", &context, ZMQ_DEALER));
	
	flags.push_back(new ReadyFlag("isFrontBind"));
	flags.push_back(new ReadyFlag("isBackBind"));
	
	items.push_back({*(sockPtrs[0]->socket), 0, ZMQ_POLLIN, 0});
}

ZMQ_Demo_Worker_Main::~ZMQ_Demo_Worker_Main(){
	for (DemoSocket *sPtr : sockPtrs){
		delete sPtr->socket;
		delete sPtr;
	}
	
	for (ReadyFlag *fPtr : flags)
		delete fPtr;
}

void ZMQ_Demo_Worker_Main::BindFront(string bindStr){
	flags[0]->val = BindPtrToAddr(sockPtrs[0]->socket, &bindStr);
}

void ZMQ_Demo_Worker_Main::BindBack(string bindStr){
	flags[1]->val = ConnectPtrToAddr(sockPtrs[1]->socket, &bindStr);
}

void ZMQ_Demo_Worker_Main::Bind(string frontStr, string backStr){
	this->BindFront(frontStr);
	this->BindBack(backStr);
}

void ZMQ_Demo_Worker_Main::DoWork()
{
	if (this->isReadyToWork()){
		cout << "Worker main ready\n";
		
		while(true){
			zmq::poll(items);
			
			if (items[0].revents && ZMQ_POLLIN){
				cout << "Have messages in front\n";
				
				string routerAddr = RecvMessage(sockPtrs[0]->socket);
				string clientAddr = RecvMessage(sockPtrs[0]->socket);
				string message = RecvMessage(sockPtrs[0]->socket);
				
				cout << "Have message: " << message << " from: " << routerAddr << " for: " << clientAddr << "\n";
				

				SendMessageWFlag(sockPtrs[1]->socket, clientAddr, ZMQ_SNDMORE);
				SendMessage(sockPtrs[1]->socket, "(worker)" + message);
				cout << "Done work and sent message to deliverer\n***\n\n";
			}
			
		}
		
	}else{
		cout << "Worker main is not ready:\n";
		for (ReadyFlag *fPtr : flags){
			cout << "    " << fPtr->name << ": " << fPtr->val << "\n";
		}
	}
}