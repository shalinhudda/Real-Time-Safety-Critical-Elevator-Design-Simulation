#include "rt.h"
#include "\Users\shali\OneDrive\4th year 1st term\CPEN 333\Lab3\Lab3\Dispatcher\Monitor.h"

CRendezvous     r1("MyRendezvous", 4);
CRendezvous     r2("MyRendezvousEnd", 4);

UINT _stdcall IoThread(void*) {
	CTypedPipe<int> PipeToIo("Pipe", 1024);
	Monitor Monitor1("Monitor1");
	Monitor Monitor2("Monitor2");
	
	r1.Wait();
	int MyData = 1;
	PipeToIo.Write(&MyData);

	r2.Wait();
	return 0;
}