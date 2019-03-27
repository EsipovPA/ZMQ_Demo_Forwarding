#include "ZMQ_Worker.h"
#include <iostream>
using namespace std;


//Testing Upp String
#include <Core/Core.h>

//===================================================================
//useful functions

bool BindByPtr(zmq::socket_t *sockPtr, string *bindStr){
	try{
		int sockType = sockPtr->getsockopt<int>(ZMQ_TYPE);
		if ((sockType == ZMQ_REP) || (sockType == ZMQ_ROUTER) || (sockType == ZMQ_DEALER)){
			cout << "I'm binding\n";
			cout << "Bind address is: " << *bindStr << "\n";
			int findRep = bindStr->find("localhost");
			cout << "findRep = " << findRep << "\n";
			cout << "sizeof(\"localhost\") = " << sizeof("localhost") << "\n";
			bindStr->replace(findRep, findRep + sizeof("localhost"), "*");
			sockPtr->bind(*bindStr);
		}else{
			cout << "I'm connectiong\n";
			sockPtr->connect(*bindStr);
		}
	}catch (zmq::error_t *zmqEx){
		throw zmqEx;
		return false;
	}
	return true;
}

bool BindPtrToAddr(zmq::socket_t *sockPtr, std::string *bindStr){
	try{
		sockPtr->bind(*bindStr);
	}catch (zmq::error_t err){
		throw err;
		return false;
	}
	return true;
}

bool ConnectPtrToAddr(zmq::socket_t *sockPtr, std::string *conStr){
	try{
		sockPtr->connect(*conStr);
	}catch (zmq::error_t err){
		throw err;
		return false;
	}
	return true;
}

bool SendMessage(zmq::socket_t *sockPtr, string *message){
	zmq::message_t msg(message->size());
	memcpy((void*) msg.data(), message->c_str(), message->size());
	try{
		return sockPtr->send(msg);
	}catch(zmq::error_t err){
		throw err;
		return false;
	}
}

bool SendMessage(zmq::socket_t *sockPtr, string message){
	return SendMessage(sockPtr, &message);
}

bool SendMessageWFlag(zmq::socket_t *sockPtr, string *message, int flag = 0){
	zmq::message_t msg(message->size());
	memcpy((void*) msg.data(), message->c_str(), message->size());
	try{
		return sockPtr->send(msg, flag);
	}catch(zmq::error_t err){
		throw err;
		return false;
	}
}

bool SendMessageWFlag(zmq::socket_t *sockPtr, string message, int flag = 0){
	return SendMessageWFlag(sockPtr, &message, flag);
}

std::string RecvMessge(zmq::socket_t *sokcet){
	zmq::message_t message;
	sokcet->recv(&message);
	
	return std::string(static_cast<char*>(message.data()), message.size());
}

void SetSocketID(zmq::socket_t *sockPtr, string newID){
	sockPtr->setsockopt(ZMQ_IDENTITY, &newID, sizeof(newID));
}

//===================================================================
//DemoSocket

DemoSocket::DemoSocket(zmq::socket_t *ptr){
	this->socket = ptr;
}

DemoSocket::DemoSocket(std::string Name, zmq::socket_t *ptr) : DemoSocket(ptr){
	this->name = Name;
}

DemoSocket::DemoSocket(zmq::context_t *context, int sockType){
	socket = new zmq::socket_t(*context, sockType);
}

DemoSocket::DemoSocket(std::string Name, zmq::context_t *context, int sockType)
	: DemoSocket(context, sockType){
	name = Name;
}

//===================================================================
//ZMQ_Demo_Networker

ZMQ_Demo_Networker::ZMQ_Demo_Networker() : context(1){
	
}

bool ZMQ_Demo_Networker::isReadyToWork(){
	for (ReadyFlag* fPtr : flags)
		if (!fPtr->val) { return false; }
	
	return true;
}

void ZMQ_Demo_Networker::OpenLog() {
	logFilePtr->open(logFileName + nwID + ".log");
}

void ZMQ_Demo_Networker::ToLog(std::string logMsg){
	//Add write to log file
	cout << "Worker: Need to put to log file this: " << logMsg << "\n";
}

void ZMQ_Demo_Networker::CloseLog() {
	logFilePtr->close();
}