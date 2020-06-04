#include "Car.h"

int main() {
	

	Car* NewCar[10];
	int* ArrayToPass[10];
	for (int i = 0; i < 10; i++) {
		ArrayToPass[i] = new int;
		*(ArrayToPass[i]) = i;
		NewCar[i] = new Car(ArrayToPass[i]);
		
		
	}
	
	for (int i = 0; i < 10; i++) {
		NewCar[i]->Resume();
	}
	NewCar[1]->Stop();
	NewCar[2]->Accelerate();
	NewCar[3]->Cruise();
	Sleep(500);
	NewCar[1]->Accelerate();
	NewCar[2]->Cruise();
	NewCar[3]->Stop();
	for (int i = 0; i < 10; i++) {
		NewCar[i]->WaitForThread();
		
	}


	
	return 0;
}