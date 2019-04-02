#include "ZMQ_Demo_Deliverer.h"

int main(){
	cout << "Deliverer working\n";
	
	ZMQ_Demo_Deliverer demoDeliverer;
	demoDeliverer.BindFront("tcp://*:5556");
	demoDeliverer.BindBack("tcp://*:5558");
	
	demoDeliverer.DoWork();
	
	
	return 1;
}