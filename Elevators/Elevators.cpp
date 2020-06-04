#include "Elevators.h"
#include "rt.h"
#include "\Users\shali\OneDrive\4th year 1st term\CPEN 333\Lab3\Lab3\Dispatcher\Monitor.h"

CRendezvous     r1("MyRendezvous", 4);
CRendezvous     r2("MyRendezvousEnd", 4);
CSemaphore PrintSync("PrintSync", 1, 1);

UINT __stdcall Elevator1Thread(void*) {
	Monitor Monitor1("Monitor1");
	CMailbox MyMailBox;
	r1.Wait();

	UINT Message;
	while (1)
	{
		if (Message = MyMailBox.GetMessageA()) {
			for (int i = 0; Message >= i; i++) {
				if (Message == i) {
					Sleep(100);
					PrintSync.Wait();
					cout << "Elevator 1 has reached floor " << i << endl;
					PrintSync.Signal();
				}
				else
				{
					PrintSync.Wait();
					cout << "Elevator 1 is going to floor " << i << endl;
					PrintSync.Signal();
					Sleep(100);
				}

			}
		}
	}


	r2.Wait();
	return 0;
};


UINT __stdcall Elevator2Thread(void*) {
	Monitor Monitor2("Monitor2");
	CMailbox MyMailBox;
	r1.Wait();

	UINT Message;
	while (1)
	{
		if (Message = MyMailBox.GetMessageA()) {
			for (int i = 0; Message >= i; i++) {
				if (Message == i) {
					Sleep(100);
					PrintSync.Wait();
					cout << "Elevator 2 has reached floor " << i << endl;
					PrintSync.Signal();
				}
				else
				{
					PrintSync.Wait();
					cout << "Elevator 2 is going to floor " << i << endl;
					PrintSync.Signal();
					Sleep(100);
				}

			}
		}
	}


	r2.Wait();
	return 0;
};