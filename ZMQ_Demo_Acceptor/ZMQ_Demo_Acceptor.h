#ifndef _ZMQ_Demo_Acceptor_ZMQ_Demo_Acceptor_h_
#define _ZMQ_Demo_Acceptor_ZMQ_Demo_Acceptor_h_

#include <ZMQ_Worker/ZMQ_Worker.h>
#include <iostream>


class ZMQ_Demo_Acceptor : private ZMQ_Demo_Networker{
public:
	ZMQ_Demo_Acceptor();
	~ZMQ_Demo_Acceptor();
	
	void BindFront(std::string bindStr);
	void ConnectBack(std::string conStr);
	
	void DoWork();
	
private:
	
	
	
};

#endif
