#include "rt.h"
#include <vector>

UINT  __stdcall SpeedoMeter(void* args) {

	for (int i = 0; i < 10; i++) {
		std::cout << "Car " << *(int*)(args) << " is speeding" <<std::endl;
		Sleep(2000);
	}
	
	return   0;

}

class Car : public ActiveClass {
	void* ArgumentToPass;
	int IntegerPointer;
	int Accelerating;
	int Cruising;
	int Stoping;

	CThread* CarSpeedoMeter = new CThread(SpeedoMeter, SUSPENDED, ArgumentToPass);

	int main() { 
		IntegerPointer = *(int*)(ArgumentToPass);
		CarSpeedoMeter->Resume();
		for (int i = 0; i < 20; i++) {
			cout << "This is the car number " << IntegerPointer << " talking from the private main function" << endl;
			if (Accelerating) {
				cout << "Car " << IntegerPointer << " is now accelerating" << endl;
			} 
			if (Cruising) {
				cout << "Car " << IntegerPointer << " is now cruising" << endl;
			}
			if (Stoping) {
				cout << "Car " << IntegerPointer << " is now stopping" << endl;
			}
			
		}
		CarSpeedoMeter->WaitForThread();

		return 0;
	};

public:
	Car(void* args) : ArgumentToPass(args) {};
	~Car() {};
	void Accelerate() { Accelerating = 1, Stoping = 0, Cruising = 0; };
	void Cruise() { Cruising = 1, Stoping = 0, Accelerating = 0; };
	void Stop() { Stoping = 1, Accelerating = 0, Cruising = 0; };
	void WaitForSpeedoMeter() { CarSpeedoMeter->WaitForThread(); };
	void SuspendSpeedoMeter() { CarSpeedoMeter->Suspend(); };
	void ResumeSpeedoMeter() { CarSpeedoMeter->Resume(); };
};

