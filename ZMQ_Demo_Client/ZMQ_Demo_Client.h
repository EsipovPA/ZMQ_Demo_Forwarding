#ifndef _ZMQ_Demo_Client_ZMQ_Demo_Client_h_
#define _ZMQ_Demo_Client_ZMQ_Demo_Client_h_

#include <ZMQ_Worker/ZMQ_Worker.h>

#include <string.h>

class ZMQ_Demo_Client : private ZMQ_Demo_Networker{
public:
	ZMQ_Demo_Client();
	~ZMQ_Demo_Client();
	
	void ConnectFront(std::string conStr);
	void DoWork();
	
	void SetID(std::string newID){
		this->SetNetworkerID(newID);
	}
	
	void SetClientLog(std::string logFileName){
		this->SetLogFileName(logFileName);
	}
	
private:
	
	
};


#endif
