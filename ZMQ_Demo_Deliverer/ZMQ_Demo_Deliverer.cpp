#include "ZMQ_Demo_Deliverer.h"

//Using this to get Sleep() function
#include <Core/Core.h>


ZMQ_Demo_Deliverer::ZMQ_Demo_Deliverer() : ZMQ_Demo_Networker(){
	sockPtrs.push_back(new DemoSocket("frontend", &context, ZMQ_ROUTER));
	sockPtrs.push_back(new DemoSocket("backend", &context, ZMQ_ROUTER));
	
	flags.push_back(new ReadyFlag("isFrontBind"));
	flags.push_back(new ReadyFlag("isBackConnected"));
	
	SetSocketID(sockPtrs[0]->socket, "deliverer");
	SetSocketID(sockPtrs[1]->socket, "deliverer");
	
	items.push_back({*(sockPtrs[0]->socket), 0, ZMQ_POLLIN, 0});
	items.push_back({*(sockPtrs[1]->socket), 0, ZMQ_POLLIN, 0});
}

ZMQ_Demo_Deliverer::~ZMQ_Demo_Deliverer(){
	for (ReadyFlag *fPtr : flags)
		delete fPtr;
	
	for (DemoSocket *sPtr : sockPtrs){
		delete sPtr->socket;
		delete sPtr;
	}
}

void ZMQ_Demo_Deliverer::BindFront(string bindStr){
	flags[0]->val = BindPtrToAddr(sockPtrs[0]->socket, &bindStr);
}

void ZMQ_Demo_Deliverer::BindBack(string conStr){
	flags[1]->val = BindPtrToAddr(sockPtrs[1]->socket, &conStr);
}

string ZMQ_Demo_Deliverer::GetClientReqest(string clientID){
	for (string req : clReqests)
		if (req == clientID)
			return req;
	return "No such slient";
}

WorkResult* ZMQ_Demo_Deliverer::GetWorkResult(string clientID){
	for (WorkResult* rPtr : wResults)
		if (rPtr->clientID == clientID)
			return rPtr;
	return NULL;
}

void ZMQ_Demo_Deliverer::DoWork(){
	if (this->isReadyToWork()){
		cout << "Deliverer is ready to work\n";
		
		while(true){
			zmq::poll(this->items);
			
			if (items[0].revents && ZMQ_POLLIN){
				cout << "Have message in front\n";
				
				//Deal with messages from clients
				string clAddr = RecvMessage(sockPtrs[0]->socket);
				RecvMessage(sockPtrs[0]->socket);
				string clReq = RecvMessage(sockPtrs[0]->socket);
				
				cout << "Have request: " << clReq << " from: " << clAddr << "\n";
				
				WorkResult *rPtr = GetWorkResult(clAddr);
				if(rPtr != NULL){
					cout << "Need to return to " << clAddr << " work result\n";
					SendMessageWFlag(sockPtrs[0]->socket, clAddr, ZMQ_SNDMORE);
					SendMessageWFlag(sockPtrs[0]->socket, "", ZMQ_SNDMORE);
					SendMessage(sockPtrs[0]->socket, "(deliverer)" + rPtr->result);
				}else{
					cout << "Have no results for: " << clAddr << "\n";
					clReqests.push_back(clAddr);
				}
			}
			
			if (items[1].revents && ZMQ_POLLIN){
				//Deal with messages from workers
				string wAddr = RecvMessage(sockPtrs[1]->socket);
				string clAddr = RecvMessage(sockPtrs[1]->socket);
				string wMessg = RecvMessage(sockPtrs[1]->socket);
				
				cout << "Have message: " << wMessg << " from: " << wAddr << " for: " <<
					clAddr << "\n";
				
				if (GetClientReqest(clAddr) != "No such slient"){
					cout << "Need to return work result to client\n";
					SendMessageWFlag(sockPtrs[0]->socket, clAddr, ZMQ_SNDMORE);
					SendMessageWFlag(sockPtrs[0]->socket, "", ZMQ_SNDMORE);
					SendMessage(sockPtrs[0]->socket, "(deliverer)" + wMessg);
				}else{
					cout << clAddr << " is not a known client\n";
					wResults.push_back(new WorkResult(clAddr, wMessg));
				}
			}
			
			//Upp::Sleep(10000000);
		}
		
		//cout << "Have message: " << clientRequest << " from: " << clientAddr << "\n";
	}else{
		cout << "Deliverer is not ready:\n";
		for (ReadyFlag *fPtr : flags)
			cout << "   " << fPtr->name << " = " << fPtr->val << "\n";
	}
}