#ifndef _ZMQ_Demo_Worler_Main_ZMQ_Demo_Worker_Main_h_
#define _ZMQ_Demo_Worler_Main_ZMQ_Demo_Worker_Main_h_

#include <iostream>
#include <vector>
#include <ZMQ_Worker/ZMQ_Worker.h>

//I'l use Upp Core here for Upp Threads
//They are just easier to use then std threads. So I do it to ease project construction
#include <Core/Core.h>

using namespace std;

class ZMQ_Demo_Worker_Main : private ZMQ_Demo_Networker{
public:
	ZMQ_Demo_Worker_Main();
	~ZMQ_Demo_Worker_Main();
	
	void BindFront(string bindStr);
	void BindBack(string bindStr);
	void Bind(string frontStr, string backStr);
	
	
	void DoWork();
private:
	
	string delivererAddr = "deliverer";
	vector<zmq::pollitem_t> items;
};

#endif
