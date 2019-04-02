#include "ZMQ_Demo_Client.h"

#include <iostream>
#include <thread>
using namespace std;

int main()
{
	ZMQ_Demo_Client testClient;
	
	testClient.ConnectFront("tcp://localhost:5555");
	testClient.SetClientLog("Cl_1_Log.txt");
	testClient.SetID("Client 1");
	testClient.DoWork();
	
	
	return 0;
}