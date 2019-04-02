#ifndef _ZMQ_Worker_ZMQ_Worker_h_
#define _ZMQ_Worker_ZMQ_Worker_h_

//May be not needed
//#include <Core/Core.h>

#include <ZMQ_431/ZMQ_431.h>
#include <string.h>
#include <vector>
#include <fstream>

struct DemoSocket{
	std::string name;
	zmq::socket_t *socket;
	
	DemoSocket(){};
	DemoSocket(zmq::socket_t *sockPtr);
	DemoSocket(std::string Name, zmq::socket_t *sockPtr);
	DemoSocket(zmq::context_t *context, int sockType);
	DemoSocket(std::string Name, zmq::context_t *context, int sockType);
	
};

struct ReadyFlag{
	std::string name;
	bool val = false;
	
	ReadyFlag(){};
	ReadyFlag(std::string Name) {this->name = Name;}
	ReadyFlag(std::string Name, bool flagVal) : ReadyFlag(Name) {this->val = flagVal;}
};


class ZMQ_Demo_Networker{
public:
	ZMQ_Demo_Networker();
	
	void SetLogFileName(std::string newName) {this->logFileName = newName;}
	void OpenLog();
	void CloseLog();
	void ToLog(std::string logMsg);
	bool isReadyToWork();
	void SetNetworkerID(std::string newNwID){
		this->nwID = newNwID;
	}
	
	//Values
	std::string logFileName;
	std::string nwID;
	zmq::context_t context;
	std::vector<DemoSocket*> sockPtrs;
	std::vector<ReadyFlag*> flags;
	
	std::ofstream *logFilePtr;
};

//Useful functions

bool BindByPtr(zmq::socket_t *sockPtr, std::string *bindStr);

bool BindPtrToAddr(zmq::socket_t *sockPtr, std::string *bindStr);

bool BindPtrToAddr(zmq::socket_t *sockPtr, std::string bindStr);

bool ConnectPtrToAddr(zmq::socket_t *sockPtr, std::string *conStr);

bool ConnectPtrToAddr(zmq::socket_t *sockPtr, std::string conStr);

bool SendMessage(zmq::socket_t *sockPtr, std::string *message);

bool SendMessage(zmq::socket_t *sockPtr, std::string message);

bool SendMessageWFlag(zmq::socket_t *sockPtr, std::string *message, int flag);

bool SendMessageWFlag(zmq::socket_t *sockPtr, std::string message, int flag);

void SetSocketID(zmq::socket_t *sockPtr, std::string newID);

std::string RecvMessage(zmq::socket_t *sokcet);

std::string RecvMessageWFlag(zmq::socket_t *socket, int flag = 0);

#endif
