#ifndef _ZMQ_Demo_Deliverer_ZMQ_Demo_Deliverer_h_
#define _ZMQ_Demo_Deliverer_ZMQ_Demo_Deliverer_h_

#include <ZMQ_Worker/ZMQ_Worker.h>
#include <vector>
#include <iostream>

using namespace std;

struct WorkResult{
	string clientID;
	string result;
	
	WorkResult() {}
	WorkResult(string ID, string res){
		this->clientID = ID;
		this->result = res;
	}
};

class ZMQ_Demo_Deliverer : private ZMQ_Demo_Networker{
public:
	ZMQ_Demo_Deliverer();
	~ZMQ_Demo_Deliverer();
	
	void BindFront(string bindStr);
	void BindBack(string conStr);
	
	void DoWork();
	
private:
	
	WorkResult *GetWorkResult(string clientID);
	string GetClientReqest(string clientID);
	
	vector<zmq::pollitem_t> items;
	vector<string> clReqests;
	vector<WorkResult*> wResults;
};

#endif
