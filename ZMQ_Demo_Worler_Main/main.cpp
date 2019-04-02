#include "ZMQ_Demo_Worker_Main.h"

int main()
{
	ZMQ_Demo_Worker_Main workerMain;
	//workerMain.Bind("tcp://*:5557", "ipc://workers");
	workerMain.Bind("tcp://*:5557", "tcp://localhost:5558");
	
	
	workerMain.DoWork();
	
	cout << "Work done\n";
	
	return 1;
}


/*
CONSOLE_APP_MAIN
{
	ZMQ_Demo_Worker_Main testWorker;
	
	//testWorker.Bind("tcp://*:5557", "tcp://*:5558");
	testWorker.Bind("tcp://*:5557", "ipc://workers");
	
	
	testWorker.DoWork();
	
	
	
	cout << "Work end\n";
	
}
*/