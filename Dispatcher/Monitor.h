#include "rt.h"

struct 	DataPoolData {
	unsigned Status;
};

class Monitor {
	string Name;
	string MyConsumer1Name = string("Consumer1") + string(Name);
	string MyConsumer2Name = string("Consumer2") + string(Name);
	string MyProducer1Name = string("Producer1") + string(Name);
	string MyProducer2Name = string("Producer2") + string(Name);
	string DataPoolName = string("DataPool") + string(Name);

	CSemaphore* MyConsumer1;
	CSemaphore* MyConsumer2;
	CSemaphore* MyProducer1;
	CSemaphore* MyProducer2;
	CDataPool* MyDataPoool;
	DataPoolData* Elevator;

public:

	Monitor(string NameToSet) : Name(NameToSet) {
		MyConsumer1 = new CSemaphore(string(MyConsumer1Name), 0);
		MyConsumer2 = new CSemaphore(string(MyConsumer2Name), 0);
		MyProducer1 = new CSemaphore(string(MyProducer1Name), 1);
		MyProducer2 = new CSemaphore(string(MyProducer2Name), 1);
		MyDataPoool = new CDataPool(string(DataPoolName), sizeof(DataPoolData));
		Elevator = (DataPoolData*)MyDataPoool->LinkDataPool();
	};

	void SetElevatorStatus(int Status) {
		MyProducer1->Wait();
		MyProducer2->Wait();
		Elevator->Status = Status;
		MyConsumer1->Signal();
		MyConsumer2->Signal();
	}

	int GetElevatorStatusConsumer1() {
		MyConsumer1->Wait();
		int VariableToReturn = Elevator->Status;
		MyProducer1->Signal();
		return VariableToReturn;
	}

	int GetElevatorStatusConsumer2() {
		MyConsumer2->Wait();
		int VariableToReturn = Elevator->Status;
		MyProducer2->Signal();
		return VariableToReturn;
	}

	~Monitor() { 
		delete MyConsumer1;
		delete MyConsumer2;
		delete MyProducer1;
		delete MyProducer2;
		delete MyDataPoool;
	};
};