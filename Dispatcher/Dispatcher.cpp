#include "rt.h"
#include "Monitor.h"
#include <list>
#include <time.h>
#include <algorithm>
#include <random>

#define WAITING_TIME_FOR_PASSENGERS_TO_PRESS_BUTTON_FROM_INSIDE 50
#define OPEN_CLOSE_TIME 200
#define ELEVATOR_1_INITIAL_FLOOR 0
#define ELEVATOR_2_INITIAL_FLOOR 0
#define FLOOR_TRAVERSING_TIME 2000
#define TERMINATION_CODE 4000
#define GOING_UP_MIN 1100
#define GOING_UP_MAX 1109
#define GOING_DOWN_MIN 1000
#define GOING_DOWN_MAX 1009
#define ELEVATOR_1_COMMAND_MIN 10
#define ELEVATOR_1_COMMAND_MAX 19
#define ELEVATOR_2_COMMAND_MIN 20
#define ELEVATOR_2_COMMAND_MAX 29
#define ELEVATOR_1_CREATE_FAULT 2000
#define ELEVATOR_1_FIX_FAULT 2001
#define ELEVATOR_2_CREATE_FAULT 3000
#define ELEVATOR_2_FIX_FAULT 3001
#define INCREMENT_FLOOR 1
#define DECREMENT_FLOOR 2
#define MOVING_UP 0
#define MOVING_DOWN 1
#define INSIDE 1
#define OUTSIDE 0
#define END_SIMNULATION 2
#define CREATE_FAULT 3
#define FIX_FAULT 4
#define START_ACTIVE_CLASS_PASSENGERS 1
#define STOP_ACTIVE_CLASS_PASSENGER 0
#define ACTIVE_CLASS_COMMAND 8000
#define MAX_ACTIVE_PASSENGERS 2
#define PASSENGER_FREQUENCY 5000
#define ELEVATOR_DOOR_OPEN 10000
#define ELEVATOR_DOOR_CLOSE 10001
#define GET_OFF_TIME 400

CRendezvous r1("MyRendezvous", 4);
CSemaphore PrintSync("PrintSync", 1, 1);
CSemaphore Elevator1UpStatusSync("Elevator1UpStatusSync", 0, 1);
CSemaphore Elevator1DownStatusSync("Elevator1DownStatusSync", 0, 1);
CSemaphore Elevator1UpStatusSync1("Elevator1UpStatusSync1", 0, 1);
CSemaphore Elevator1DownStatusSync1("Elevator1DownStatusSync1", 0, 1);
CSemaphore Elevator2UpStatusSync("Elevator2UpStatusSync", 0, 1);
CSemaphore Elevator2DownStatusSync("Elevator2DownStatusSync", 0, 1);
CSemaphore Elevator2UpStatusSync1("Elevator2UpStatusSync1", 0, 1);
CSemaphore Elevator2DownStatusSync1("Elevator2DownStatusSync1", 0, 1);
CSemaphore ActiveObjectSync("ActiveObjectSync", 1, 1);
CSemaphore ActiveObjectCountSync("ActiveObjectCountSync", MAX_ACTIVE_PASSENGERS, MAX_ACTIVE_PASSENGERS);
CSemaphore Elevator1DirectionProtectionSync("Elevator1DirectionProtectionSync", 1, 1);
CSemaphore Elevator2DirectionProtectionSync("Elevator2DirectionProtectionSync", 1, 1);

list<int> Elevator1Direction;
list<int> Elevator2Direction;

void clearGoingUpDown(int elevator, int UPorDOWN) {
	PrintSync.Wait();
	if (elevator == 1 && UPorDOWN == 11) {
		MOVE_CURSOR(30, 2);
		printf("   ");
		MOVE_CURSOR(30, 3);
		printf("   ");
	}
	if (elevator == 1 && UPorDOWN == 10) {
		MOVE_CURSOR(30, 2);
		printf("   ");
		MOVE_CURSOR(30, 3);
		printf("   ");
	}
	if (elevator == 2 && UPorDOWN == 11) {
		MOVE_CURSOR(112, 2);
		printf("   ");
		MOVE_CURSOR(112, 3);
		printf("   ");
	}
	if (elevator == 2 && UPorDOWN == 10) {
		MOVE_CURSOR(112, 2);
		printf("   ");
		MOVE_CURSOR(112, 3);
		printf("   ");
	}
	PrintSync.Signal();
}

void ElevatorsWorksorBroken(int elevator, int workorBroken) {
	PrintSync.Wait();
	if (elevator == 1 && workorBroken == 1) {
		MOVE_CURSOR(5, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("Elevator Operational");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	if (elevator == 1 && workorBroken == 0) {
		MOVE_CURSOR(5, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("Elevator Broken     ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	}

	if (elevator == 2 && workorBroken == 1) {
		MOVE_CURSOR(118, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("Elevator Operational");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	if (elevator == 2 && workorBroken == 0) {
		MOVE_CURSOR(118, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("Elevator Broken     ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	}
	PrintSync.Signal();
}
void ElevatorsWorksorBroken(int workorBroken) {
	PrintSync.Wait();
	if (workorBroken == 1) {
		MOVE_CURSOR(5, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("Elevator Operational");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	if (workorBroken == 0) {
		MOVE_CURSOR(5, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("Elevator Broken     ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	}
	PrintSync.Signal();
}

void setConsoleSize() {
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);

	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 5000, 5000, TRUE);
}
void drawGoingUpDown(int elevator, int UPorDOWN) {
	PrintSync.Wait();
	if (elevator == 1 && UPorDOWN == 11) {
		MOVE_CURSOR(30, 2);
		printf(" * ");
		MOVE_CURSOR(30, 3);
		printf("***");
	}
	if (elevator == 1 && UPorDOWN == 10) {
		MOVE_CURSOR(30, 2);
		printf("***");
		MOVE_CURSOR(30, 3);
		printf(" * ");
	}
	if (elevator == 2 && UPorDOWN == 11) {
		MOVE_CURSOR(112, 2);
		printf(" * ");
		MOVE_CURSOR(112, 3);
		printf("***");
	}
	if (elevator == 2 && UPorDOWN == 10) {
		MOVE_CURSOR(112, 2);
		printf("***");
		MOVE_CURSOR(112, 3);
		printf(" * ");
	}
	PrintSync.Signal();
}

void doorOpenClose(int elevator, int openOrClose) {
	if (elevator == 1 && openOrClose == ELEVATOR_DOOR_OPEN) {
		PrintSync.Wait();
		//MOVE_CURSOR(50, 2);
		//cout << "Elevator 1 Door Opening";		
		MOVE_CURSOR(56, 3);
		for (int r = 56; r >= 50; r--) {
			MOVE_CURSOR(r, 3);
			printf(" ");
			SLEEP(150);
		}
		PrintSync.Signal();
	}
	if (elevator == 1 && openOrClose == ELEVATOR_DOOR_CLOSE) {
		PrintSync.Wait();
		//MOVE_CURSOR(50, 2);
		//cout << "Elevator 1 Door closing";
		MOVE_CURSOR(50, 3);
		for (int r = 50; r <= 56; r++) {
			MOVE_CURSOR(r, 3);
			printf("%c", 219);
			SLEEP(150);
		}
		PrintSync.Signal();
	}
	if (elevator == 2 && openOrClose == ELEVATOR_DOOR_OPEN) {
		PrintSync.Wait();
		//MOVE_CURSOR(80, 2);
		//cout << "Elevator 2 Door Opening";
		MOVE_CURSOR(86, 3);
		for (int r = 86; r >= 80; r--) {
			MOVE_CURSOR(r, 3);
			printf(" ");
			SLEEP(150);
		}
		PrintSync.Signal();
	}
	if (elevator == 2 && openOrClose == ELEVATOR_DOOR_CLOSE) {
		PrintSync.Wait();
		//MOVE_CURSOR(80, 2);
		//cout << "Elevator 2 Door closing";
		MOVE_CURSOR(80, 3);
		for (int r = 80; r <= 86; r++) {
			MOVE_CURSOR(r, 3);
			printf("%c", 219);
			SLEEP(150);
		}
		PrintSync.Signal();
	}
}
void drawFloors() {
	int x = 70;
	MOVE_CURSOR(x, 10);
	printf("Floor9\n");
	MOVE_CURSOR(x, 11);
	printf("Floor8\n");
	MOVE_CURSOR(x, 12);
	printf("Floor7\n");
	MOVE_CURSOR(x, 13);
	printf("Floor6\n");
	MOVE_CURSOR(x, 14);
	printf("Floor5\n");
	MOVE_CURSOR(x, 15);
	printf("Floor4\n");
	MOVE_CURSOR(x, 16);
	printf("Floor3\n");
	MOVE_CURSOR(x, 17);
	printf("Floor2\n");
	MOVE_CURSOR(x, 18);
	printf("Floor1\n");
	MOVE_CURSOR(x, 19);
	printf("Floor0\n");
}

void drawUpDown_ReqestsOutside(int floor, int upDown) {
	int x_coordUP = 62, x_coordDOWN = 65, y_coord = 10;
	//1 is up and 0 is down
	PrintSync.Wait();
	if (floor == 9 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord);
		printf("UP");
	}
	if (floor == 9 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord);
		printf("DOWN");
	}
	if (floor == 8 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 1);
		printf("UP");
	}
	if (floor == 8 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 1);
		printf("DOWN");
	}
	if (floor == 7 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 2);
		printf("UP");
	}
	if (floor == 7 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 2);
		printf("DOWN");
	}
	if (floor == 6 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 3);
		printf("UP");
	}
	if (floor == 6 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 3);
		printf("DOWN");
	}
	if (floor == 5 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 4);
		printf("UP");
	}
	if (floor == 5 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 4);
		printf("DOWN");
	}
	if (floor == 4 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 5);
		printf("UP");
	}
	if (floor == 4 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 5);
		printf("DOWN");
	}
	if (floor == 3 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 6);
		printf("UP");
	}
	if (floor == 3 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 6);
		printf("DOWN");
	}
	if (floor == 2 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 7);
		printf("UP");
	}
	if (floor == 2 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 7);
		printf("DOWN");
	}
	if (floor == 1 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 8);
		printf("UP");
	}
	if (floor == 1 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 8);
		printf("DOWN");
	}

	if (floor == 0 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 9);
		printf("UP");
	}
	if (floor == 0 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 9);
		printf("DOWN");
	}
	PrintSync.Signal();

}

void clearUpDown_RequestsOutside(int floor, int upDown) {
	int x_coordUP = 62, x_coordDOWN = 65, y_coord = 10;
	//1 is up and 0 is down
	PrintSync.Wait();
	if (floor == 9 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord);
		printf("  ");
	}
	if (floor == 9 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord);
		printf("    ");
	}
	if (floor == 8 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 1);
		printf("  ");
	}
	if (floor == 8 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 1);
		printf("    ");
	}
	if (floor == 7 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 2);
		printf("  ");
	}
	if (floor == 7 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 2);
		printf("    ");
	}
	if (floor == 6 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 3);
		printf("  ");
	}
	if (floor == 6 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 3);
		printf("    ");
	}
	if (floor == 5 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 4);
		printf("  ");
	}
	if (floor == 5 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 4);
		printf("    ");
	}
	if (floor == 4 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 5);
		printf("  ");
	}
	if (floor == 4 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 5);
		printf("    ");
	}
	if (floor == 3 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 6);
		printf("  ");
	}
	if (floor == 3 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 6);
		printf("    ");
	}
	if (floor == 2 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 7);
		printf("  ");
	}
	if (floor == 2 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 7);
		printf("    ");
	}
	if (floor == 1 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 8);
		printf("  ");
	}
	if (floor == 1 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 8);
		printf("    ");
	}

	if (floor == 0 && upDown == 11) {
		MOVE_CURSOR(x_coordUP, y_coord + 9);
		printf("  ");
	}
	if (floor == 0 && upDown == 10) {
		MOVE_CURSOR(x_coordDOWN, y_coord + 9);
		printf("    ");
	}
	PrintSync.Signal();

}

void drawfloorNumbers(int elevator, int floor) {
	PrintSync.Wait();
	if (elevator == 1 && floor == 0) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("*  *");
		MOVE_CURSOR(38, 4);
		printf("*  *");
		MOVE_CURSOR(38, 5);
		printf("*  *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 0) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("*  *");
		MOVE_CURSOR(105, 4);
		printf("*  *");
		MOVE_CURSOR(105, 5);
		printf("*  *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 1) {
		MOVE_CURSOR(38, 2);
		printf("*** ");
		MOVE_CURSOR(38, 3);
		printf(" ** ");
		MOVE_CURSOR(38, 4);
		printf(" ** ");
		MOVE_CURSOR(38, 5);
		printf(" **  ");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 1) {
		MOVE_CURSOR(105, 2);
		printf("*** ");
		MOVE_CURSOR(105, 3);
		printf(" ** ");
		MOVE_CURSOR(105, 4);
		printf(" ** ");
		MOVE_CURSOR(105, 5);
		printf(" ** ");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 2) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("   *");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("*   ");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 2) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("   *");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("*   ");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 3) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("   *");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("   *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 3) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("   *");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("   *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}

	if (elevator == 1 && floor == 4) {
		MOVE_CURSOR(38, 2);
		printf("*  *");
		MOVE_CURSOR(38, 3);
		printf("*  *");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("   *");
		MOVE_CURSOR(38, 6);
		printf("   *");
	}
	if (elevator == 2 && floor == 4) {
		MOVE_CURSOR(105, 2);
		printf("*  *");
		MOVE_CURSOR(105, 3);
		printf("*  *");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("   *");
		MOVE_CURSOR(105, 6);
		printf("   *");
	}
	if (elevator == 1 && floor == 5) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("*   ");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("   *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 5) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("*   ");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("   *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 6) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("*   ");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("*  *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 6) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("*   ");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("*  *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 7) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("   *");
		MOVE_CURSOR(38, 4);
		printf("   *");
		MOVE_CURSOR(38, 5);
		printf("   *");
		MOVE_CURSOR(38, 6);
		printf("   *");
	}
	if (elevator == 2 && floor == 7) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("   *");
		MOVE_CURSOR(105, 4);
		printf("   *");
		MOVE_CURSOR(105, 5);
		printf("   *");
		MOVE_CURSOR(105, 6);
		printf("   *");
	}
	if (elevator == 1 && floor == 8) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("*  *");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("*  *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 8) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("*  *");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("*  *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	if (elevator == 1 && floor == 9) {
		MOVE_CURSOR(38, 2);
		printf("****");
		MOVE_CURSOR(38, 3);
		printf("*  *");
		MOVE_CURSOR(38, 4);
		printf("****");
		MOVE_CURSOR(38, 5);
		printf("   *");
		MOVE_CURSOR(38, 6);
		printf("****");
	}
	if (elevator == 2 && floor == 9) {
		MOVE_CURSOR(105, 2);
		printf("****");
		MOVE_CURSOR(105, 3);
		printf("*  *");
		MOVE_CURSOR(105, 4);
		printf("****");
		MOVE_CURSOR(105, 5);
		printf("   *");
		MOVE_CURSOR(105, 6);
		printf("****");
	}
	PrintSync.Signal();

}

list<int> GetIoInput() {
	char input;
	char floor_at;
	char desired_elevator;
	char destination_floor;
	int some_flag = 1;
	string s;
	list<int> customerInput;

	input = _getch();

	if (input == 'U' || input == 'u' || input == 'D' || input == 'd') {
		customerInput.push_back(OUTSIDE);

		if (input == 'U' || input == 'u') {
			s = "11";
		}
		else if (input == 'D' || input == 'd') {
			s = "10";
		}

		floor_at = _getch(); 
		if (floor_at == '+' || floor_at == '-') {
			some_flag = 0;
		}
		if(some_flag && isdigit(floor_at)) {
			while (!isdigit(floor_at))
			{
				PrintSync.Wait();
				MOVE_CURSOR(10, 20);
				std::cout << "Error invalid input please try again " << std::endl;
				std::cout << "Please enter at number between 0-9: ";
				PrintSync.Signal();
				floor_at = _getch();
			}
			string k(1, floor_at);
			string t = '0' + k;

			string customerIn;
			int InputFloor, InputUpDown;
			InputUpDown = stoi(s);
			InputFloor = stoi(k);
			customerIn = s + t;
			
			customerInput.push_back(stoi(customerIn));
			if (customerInput.back() == GOING_UP_MAX || customerInput.back() == GOING_DOWN_MIN) {
				PrintSync.Wait();
				MOVE_CURSOR(10, 20);
				cout << "Invalid Input, U9 and D0 are not accepted" << endl;
				PrintSync.Signal();
				customerInput = GetIoInput();
			}

			drawUpDown_ReqestsOutside(InputFloor,InputUpDown);
		}
	}

	if (input == 'D' || input == 'd' && !some_flag) {
		customerInput.pop_back();
		customerInput.push_back(ACTIVE_CLASS_COMMAND);
		if (floor_at == '+') {
			customerInput.push_back(START_ACTIVE_CLASS_PASSENGERS);
		}
		else if (floor_at == '-') {
			customerInput.push_back(STOP_ACTIVE_CLASS_PASSENGER);
		}
		else {
			PrintSync.Wait();
			MOVE_CURSOR(10, 20);
			cout << "Invalid input" << endl;
			PrintSync.Signal();
			customerInput = GetIoInput();
		}
	}

	if (input == '1' || input == '2') {
		customerInput.push_back(INSIDE);

		destination_floor = _getch();
		if (!isdigit(destination_floor)) {
			PrintSync.Wait();
			MOVE_CURSOR(10, 20);
			std::cout << "Error invalid input please try again " << endl;
			cout << "Please enter a number between 0 and 9" << endl;
			PrintSync.Signal();
			customerInput = GetIoInput();
		}

		string k(1, input);
		string p(1, destination_floor);
		string customerInElevator = k + p;
		customerInput.push_back(stoi(customerInElevator));
	}

	if (input == 'E' || input == 'e') {
		customerInput.push_back(END_SIMNULATION);
		char next_char;

		next_char = _getch();

		if (next_char == 'E' || next_char == 'e') {
			customerInput.push_back(TERMINATION_CODE);
		}
		else {
			PrintSync.Wait();
			MOVE_CURSOR(10, 20);
			cout << "Not a valid response" << endl;
			PrintSync.Signal();
			customerInput = GetIoInput();
		}
	}

	if (input == '-') {
		customerInput.push_back(CREATE_FAULT);
		char next_char;

		next_char = _getch();

		if (next_char == '1') {
			customerInput.push_back(ELEVATOR_1_CREATE_FAULT);
		}
		else if (next_char == '2') {
			customerInput.push_back(ELEVATOR_2_CREATE_FAULT);
		}
		else {
			PrintSync.Wait();
			MOVE_CURSOR(10, 20);
			cout << "Not a valid response" << endl;
			PrintSync.Signal();
			customerInput = GetIoInput();
		}
	}

	if (input == '+') {
		customerInput.push_back(FIX_FAULT);
		char next_char;

		next_char = _getch();

		if (next_char == '1') {
			customerInput.push_back(ELEVATOR_1_FIX_FAULT);
		}
		else if (next_char == '2') {
			customerInput.push_back(ELEVATOR_2_FIX_FAULT);
		}
		else {
			PrintSync.Wait();
			MOVE_CURSOR(10, 20);
			cout << "Not a valid response" << endl;
			customerInput = GetIoInput();
			PrintSync.Signal();
		}
	}

	return customerInput;
}

int Elevator1StatusForIo;
int Elevator2StatusForIo;

UINT _stdcall Elevator1StatusThreadForIo(void*) {
	Monitor Monitor1("Monitor1");

	while (1) {
		Elevator1StatusForIo = Monitor1.GetElevatorStatusConsumer2();
	}
	return 0;
}

UINT _stdcall Elevator2StatusThreadForIo(void*) {
	Monitor Monitor2("Monitor2");

	while (1) {
		Elevator2StatusForIo = Monitor2.GetElevatorStatusConsumer2();
	}
	return 0;
}
int major_flag = 0;

#define ACTIVE_CLASS_GOING_UP 1
#define ACTIVE_CLASS_GOING_DOWN 0
#define ACTIVE_CLASS_GOING_UP_MIN 1100
#define ACTIVE_CLASS_GOING_DOWN_MIN 1000

#define ACTIVE_CLASS_ELEVATOR_1 10
#define ACTIVE_CLASS_ELEVATOR_2 20
int major_flag_2 = 0;

CSemaphore ProtectionSync1("ProtectionSync1", 1, 1);
CSemaphore ProtectionSync2("ProtectionSync2", 1, 1);

CSemaphore BrandNewFlagSync("BrandNewFlagSync", 1, 1);

class ActiveObjects : public ActiveClass {

	int main() {
		//ProtectionSync1.Wait();
		srand(time(NULL));
		int Direction;
		int FloorToGo;
		ProtectionSync1.Wait();
		major_flag_2 = 0;
		ProtectionSync1.Signal();
		int MyData;
		CTypedPipe<int> PipeToIo("ActiveClassPipe", 1024);

		int UpOrDownRequest;
		int UpOrDown = rand() % 2;
		int Floor = rand() % 10;
		int FloorFromInside =  rand() % 10;

		if (UpOrDown == ACTIVE_CLASS_GOING_UP) {
			// Going Up
			while (Floor == 9) {
				Floor = rand() % 10;
			}
			UpOrDownRequest = ACTIVE_CLASS_GOING_UP_MIN + Floor;
			while (FloorFromInside <= Floor) {
				FloorFromInside = rand() % 10;
			}
		}
		else if (UpOrDown == ACTIVE_CLASS_GOING_DOWN) {
			// Going Down
			while (Floor == 0) {
				Floor = rand() % 10;
			}
			UpOrDownRequest = ACTIVE_CLASS_GOING_DOWN_MIN + Floor;
			while (FloorFromInside >= Floor) {
				FloorFromInside = rand() % 10;
			}
		}

		if (UpOrDownRequest >= GOING_UP_MIN && UpOrDownRequest <= GOING_UP_MAX) {
			FloorToGo = UpOrDownRequest - GOING_UP_MIN;
			Direction = MOVING_UP;
			drawUpDown_ReqestsOutside(FloorToGo, 11);
		}
		if (UpOrDownRequest >= GOING_DOWN_MIN && UpOrDownRequest <= GOING_DOWN_MAX) {
			FloorToGo = UpOrDownRequest - GOING_DOWN_MIN;
			Direction = MOVING_DOWN;
			drawUpDown_ReqestsOutside(FloorToGo, 10);
		}

		ActiveObjectSync.Wait();
		PipeToIo.Write(&UpOrDownRequest);
		ActiveObjectSync.Signal();

		//ProtectionSync2.Wait();
		string Sem;
		string Sem2;
		while(1) {
			Elevator1DirectionProtectionSync.Wait();
			if (Elevator1StatusForIo == FloorToGo && Elevator1Direction.size() != 0) {
				if (Elevator1Direction.back() == Direction) {
					Elevator1DirectionProtectionSync.Signal();
					if (Direction == MOVING_UP) {
						//////////////////////////////////
						BrandNewFlagSync.Wait();
						Sem = (string(string("Elevator1UpConditionF") + string(to_string(Elevator1StatusForIo)) + string("Sem")));
						Sem2 = (string(string("Elevator1UpConditionF") + string(to_string(Elevator1StatusForIo)) + string("Cond")));
						CSemaphore Semaphore(Sem, 0, 1);
						Semaphore.Signal();
						BrandNewFlagSync.Signal();
						//////////////////////////////////
						CCondition Name(Sem2);
						Name.Wait();
						ActiveObjectSync.Wait();
						MyData = FloorFromInside+10;
						ActiveObjectSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
						break;
					}
					else {
						//////////////////////////////////
						BrandNewFlagSync.Wait();
						Sem = (string(string("Elevator1") + string("DownConditionF") + string(to_string(Elevator1StatusForIo) + string("Sem"))));
						Sem2 = (string(string("Elevator1") + string("DownConditionF") + string(to_string(Elevator1StatusForIo))+string("Cond")));
						CSemaphore Semaphore(Sem, 0, 1);
						Semaphore.Signal();
						BrandNewFlagSync.Signal();
						//////////////////////////////////
						CCondition Name(Sem2);
						Name.Wait();
						ActiveObjectSync.Wait();
						MyData = FloorFromInside+10;
						ActiveObjectSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
						break;
					}
				}

			}
			if (Elevator1DirectionProtectionSync.Read() == 0) {
				Elevator1DirectionProtectionSync.Signal();
			}
			//Elevator1DirectionProtectionSync.Signal();
			Elevator2DirectionProtectionSync.Wait();
			if (Elevator2StatusForIo == FloorToGo && Elevator2Direction.size() != 0) {
				if (Elevator2Direction.back() == Direction) {
					Elevator2DirectionProtectionSync.Signal();
					if (Direction == MOVING_UP) {
						//////////////////////////////////
						BrandNewFlagSync.Wait();
						Sem = (string(string("Elevator2") + string("UpConditionF") + string(to_string(Elevator2StatusForIo)+ string("Sem"))));
						Sem2 = (string(string("Elevator2") + string("UpConditionF") + string(to_string(Elevator2StatusForIo)+string("Cond"))));
						CSemaphore Semaphore(Sem, 0, 1);
						Semaphore.Signal();
						BrandNewFlagSync.Signal();
						//////////////////////////////////
						CCondition Name(Sem2);
						Name.Wait();
						ActiveObjectSync.Wait();
						MyData = FloorFromInside+20;
						ActiveObjectSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
						break;
					}
					else {
						//////////////////////////////////
						BrandNewFlagSync.Wait();
						Sem = (string(string("Elevator2") + string("DownConditionF") + string(to_string(Elevator2StatusForIo) + string("Sem"))));
						Sem2 = (string(string("Elevator2") + string("DownConditionF") + string(to_string(Elevator2StatusForIo) + string("Cond"))));
						CSemaphore Semaphore(Sem, 0, 1);
						Semaphore.Signal();
						BrandNewFlagSync.Signal();
						//////////////////////////////////
						CCondition Name(Sem2);
						Name.Wait();
						ActiveObjectSync.Wait();
						MyData = FloorFromInside+20;
						ActiveObjectSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
						break;
					}
				}
			}
			if (Elevator2DirectionProtectionSync.Read() == 0) {
				Elevator2DirectionProtectionSync.Signal();
			}
		}
		ActiveObjectSync.Wait();
		PipeToIo.Write(&MyData);
		ActiveObjectSync.Signal();

		//ProtectionSync2.Signal();
		return 0;
	}


public:
	ActiveObjects() {}
	~ActiveObjects() {}
};

UINT _stdcall GetCharacterThread(void*) {
	CTypedPipe<int> PipeFromCharThread("PipeFromCharThread", 1024);
	CMailbox MyMailBox;
	list<int> Input;

	while(1) {
		major_flag = 0;
		int MyData;
		int flag = 0;
		int termination_flag = 0;
		int FloorToGo = 0;
		int Direction = -1;
		UINT Message;

		Input = GetIoInput();

		if (Input.size() != 0) {
			if (Input.front() == END_SIMNULATION) {
				MyData = Input.back();
				flag = 1;
				termination_flag = 1;
			}
			else if (!MyMailBox.TestForMessage() && Input.front() == OUTSIDE) {
				MyData = Input.back();
				flag = 1;
			}
			else if (MyMailBox.TestForMessage() && Input.front() == INSIDE) {
				Message = MyMailBox.GetMessageA();

				MyData = Input.back();
				/*
				if (MyData >= ELEVATOR_1_COMMAND_MIN && MyData <= ELEVATOR_1_COMMAND_MAX) {
					FloorToGo = MyData - ELEVATOR_1_COMMAND_MIN;
				}
				if (MyData >= ELEVATOR_2_COMMAND_MIN && MyData <= ELEVATOR_2_COMMAND_MAX) {
					FloorToGo = MyData - ELEVATOR_2_COMMAND_MIN;
				}
				*/
				/*
				if (MyData >= GOING_UP_MIN && MyData <= GOING_UP_MAX) {
					FloorToGo = MyData - GOING_UP_MIN;
					Direction = MOVING_UP;
				}
				if (MyData >= GOING_DOWN_MIN && MyData <= GOING_DOWN_MAX) {
					FloorToGo = MyData - GOING_DOWN_MIN;
					Direction = MOVING_DOWN;
				}
				*/
				Elevator1DirectionProtectionSync.Wait();
				if (Elevator1Direction.size() != 0) {

						if (Elevator1Direction.back() == MOVING_UP && MyData >= ELEVATOR_1_COMMAND_MIN && MyData <= ELEVATOR_1_COMMAND_MAX) {
							Elevator1DirectionProtectionSync.Signal();
							major_flag = 1;
							CCondition Name(string(string("Elevator1") + string("UpConditionF") + string(to_string(Elevator1StatusForIo)) + string("Cond")));
							Name.Wait();
							MyData = Input.back();
							doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
							flag = 1;
						}
						else if(Elevator1Direction.back() == MOVING_DOWN && MyData >= ELEVATOR_1_COMMAND_MIN && MyData <= ELEVATOR_1_COMMAND_MAX) {
							Elevator1DirectionProtectionSync.Signal();
							major_flag = 1;
							CCondition Name(string(string("Elevator1") + string("DownConditionF") + string(to_string(Elevator1StatusForIo)) + string("Cond")));
							Name.Wait();
							MyData = Input.back();
							doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
							flag = 1;
						}


				}
				if (Elevator1DirectionProtectionSync.Read() == 0) {
					Elevator1DirectionProtectionSync.Signal();
				}
				Elevator2DirectionProtectionSync.Wait();
				if (Elevator2Direction.size() != 0) {
						if (Elevator2Direction.back() == MOVING_UP && MyData >= ELEVATOR_2_COMMAND_MIN && MyData <= ELEVATOR_2_COMMAND_MAX) {
							Elevator2DirectionProtectionSync.Signal();
							major_flag = 1;
							CCondition Name(string(string("Elevator2") + string("UpConditionF") + string(to_string(Elevator2StatusForIo)) + string("Cond")));
							Name.Wait();
							MyData = Input.back();
							doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
							flag = 1;
						}
						else if(Elevator2Direction.back() == MOVING_DOWN && MyData >= ELEVATOR_2_COMMAND_MIN && MyData <= ELEVATOR_2_COMMAND_MAX){
							Elevator2DirectionProtectionSync.Signal();
							major_flag = 1;
							CCondition Name(string(string("Elevator2") + string("DownConditionF") + string(to_string(Elevator2StatusForIo)) + string("Cond")));
							Name.Wait();
							MyData = Input.back();
							doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
							flag = 1;
						}
				}
				if (Elevator2DirectionProtectionSync.Read() == 0) {
					Elevator2DirectionProtectionSync.Signal();
				}
				
			}
			else if (!MyMailBox.TestForMessage() && Input.front() == CREATE_FAULT || Input.front() == FIX_FAULT || Input.front() == ACTIVE_CLASS_COMMAND) {
				MyData = Input.back();
				flag = 1;
			}

			if (flag) {
				PipeFromCharThread.Write(&MyData);
			}

			if (termination_flag) {
				break;
			}

		}
	}

	return 0;
}

UINT _stdcall ActiveObjectsThread(void*) {
	CTypedPipe<int> PipeFromAoThread("PipeFromActiveObjectsThread", 1024);
	CTypedPipe<int> PipeFromActiveObject("ActiveClassPipe", 1024);
	int ActiveObjectData;
	int PipeData;
	int i = 0;
	ActiveObjects* NewObject[MAX_ACTIVE_PASSENGERS];

	while(1) {

		ActiveObjectSync.Wait();
		if (PipeFromActiveObject.TestForData() >= 1) {
			PipeFromActiveObject.Read(&PipeData);
			ActiveObjectData = PipeData;
			//PrintSync.Wait();
			//printf("This is the data recieved from Active Object Pipe: %d\n", PipeData);
			//PrintSync.Signal();
			PipeFromAoThread.Write(&ActiveObjectData);
		}
		ActiveObjectSync.Signal();

		if (ActiveObjectCountSync.Read()) {
			if (i == 0) {
				
			}
			else {
				Sleep(PASSENGER_FREQUENCY);
			}
			ActiveObjectCountSync.Wait();
			NewObject[i] = new ActiveObjects();
			NewObject[i]->Resume();

			i++;
		}
	}
	return 0;
}
int brand_new_flag = 0;

UINT _stdcall IoThread(void*) {
	CTypedPipe<int> PipeToIo("Pipe", 1024);
	CTypedPipe<int> PipeFromAoThread("PipeFromActiveObjectsThread", 1024);
	CTypedPipe<int> PipeFromCharThread("PipeFromCharThread", 1024);
	Monitor Monitor1("Monitor1");
	Monitor Monitor2("Monitor2");
	CMailbox MyMailBox;
	CThread Elevator2StatusThreadForIo(Elevator2StatusThreadForIo, ACTIVE);
	CThread Elevator1StatusThreadForIo(Elevator1StatusThreadForIo, ACTIVE);
	UINT Message;
	CThread ActiveObjectsThread(ActiveObjectsThread, SUSPENDED);

	CThread GetCharacterThread(GetCharacterThread, ACTIVE);
	r1.Wait();

	while (TRUE) {
		int PipeData;

		ActiveObjectSync.Wait();
		if (PipeFromAoThread.TestForData() >= 1 && !MyMailBox.TestForMessage()) {
			int data;
			PipeFromAoThread.Read(&data);
			PipeToIo.Write(&data);
		}
		ActiveObjectSync.Signal();

		if (MyMailBox.TestForMessage()) {
			Message = MyMailBox.GetMessageA();
			if(!brand_new_flag) {
				GetCharacterThread.Post(Message);
			}
		}

		if (PipeFromCharThread.TestForData() >= 1 && !MyMailBox.TestForMessage()) {
			int data;
			PipeFromCharThread.Read(&data);
			if (data == START_ACTIVE_CLASS_PASSENGERS || data == STOP_ACTIVE_CLASS_PASSENGER) {
				if (data == START_ACTIVE_CLASS_PASSENGERS) {
					BrandNewFlagSync.Wait();
					brand_new_flag = 1;
					BrandNewFlagSync.Signal();
					ActiveObjectsThread.Resume();
				}
				if(data == STOP_ACTIVE_CLASS_PASSENGER) {
					BrandNewFlagSync.Wait();
					brand_new_flag = 0;
					BrandNewFlagSync.Signal();
					ActiveObjectsThread.Suspend();
					for (int i = 0; i < MAX_ACTIVE_PASSENGERS; i++) {
						ActiveObjectCountSync.Signal();
					}
				}
			}
			else {
				PipeToIo.Write(&data);
			}

		}


	}

	return 0;
}

int Elevator1Status;
int Elevator2Status;

UINT __stdcall Elevator1Thread(void*) {
	Monitor Monitor1("Monitor1");
	CMailbox MyMailBox;
	Monitor1.SetElevatorStatus(ELEVATOR_1_INITIAL_FLOOR);
	doorOpenClose(1, ELEVATOR_DOOR_OPEN);
	ElevatorsWorksorBroken(1, 1);
	r1.Wait();
	int flag = 0;
	UINT Message;
	while (1)
	{

		if (Message = MyMailBox.GetMessageA()) {
			if (Message != TERMINATION_CODE) {
				if (Message == 1 && Elevator1Status >= 0 && flag == 0) {
					Elevator1UpStatusSync.Wait();
					Sleep(FLOOR_TRAVERSING_TIME);
					if (Elevator1Status + 1 != 9) {
						drawGoingUpDown(1, 11);
					}
					Monitor1.SetElevatorStatus(Elevator1Status + 1);
					Elevator1UpStatusSync1.Signal();
				}
				else if (Message == 2 && Elevator1Status >= 0 && flag == 0) {
					Elevator1DownStatusSync.Wait();
					Sleep(FLOOR_TRAVERSING_TIME);
					if (Elevator1Status - 1 != 0) {
						drawGoingUpDown(1, 10);
					}
					Monitor1.SetElevatorStatus(Elevator1Status - 1);
					Elevator1DownStatusSync1.Signal();
				}
				else if (Message == ELEVATOR_1_CREATE_FAULT) {
					flag = 1;
				}
				else if (Message == ELEVATOR_1_FIX_FAULT) {
					flag = 0;
				}
			}
			else {
				break;
			}
		}
	}

	return 0;
};

UINT __stdcall Elevator2Thread(void*) {
	Monitor Monitor2("Monitor2");
	CMailbox MyMailBox;
	Monitor2.SetElevatorStatus(ELEVATOR_2_INITIAL_FLOOR);
	doorOpenClose(2, ELEVATOR_DOOR_OPEN);
	ElevatorsWorksorBroken(2, 1);
	r1.Wait();
	int flag = 0;
	UINT Message;
	while (1)
	{
		if (Message = MyMailBox.GetMessageA()) {
			if (Message != TERMINATION_CODE) {
				if (Message == 1 && Elevator1Status >= 0 && flag == 0) {
					Elevator2UpStatusSync.Wait();
					Sleep(FLOOR_TRAVERSING_TIME);
					if (Elevator2Status + 1 != 9) {
						drawGoingUpDown(2, 11);
					}
					Monitor2.SetElevatorStatus(Elevator2Status + 1);
					Elevator2UpStatusSync1.Signal();
				}
				else if (Message == 2 && Elevator1Status >= 0 && flag == 0) {
					Elevator2DownStatusSync.Wait();
					Sleep(FLOOR_TRAVERSING_TIME);
					if (Elevator2Status - 1 != 0) {
						drawGoingUpDown(2, 10);
					}
					Monitor2.SetElevatorStatus(Elevator2Status - 1);
					Elevator2DownStatusSync1.Signal();
				}
				else if (Message == ELEVATOR_2_CREATE_FAULT) {
					flag = 1;
				}
				else if (Message == ELEVATOR_2_FIX_FAULT) {
					flag = 0;
				}
			}
			else {
				break;
			}
		}
	}

	return 0;
};

UINT _stdcall Elevator1StatusThread(void*) {
	Monitor Monitor1("Monitor1");
	CMailbox MyMailBox;
	while (1) {
		if (MyMailBox.TestForMessage()) {
			if (MyMailBox.GetMessageA() == TERMINATION_CODE) {
				break;
			}
		}
		Elevator1Status = Monitor1.GetElevatorStatusConsumer1();
		//PrintSync.Wait();
		//std::cout << "Elevator 1 Status " << Elevator1Status << endl;
		//PrintSync.Signal();
		drawfloorNumbers(1, Elevator1Status);
	}
	return 0;
}

UINT _stdcall Elevator2StatusThread(void*) {
	Monitor Monitor2("Monitor2");
	CMailbox MyMailBox;
	while(1) {
		if (MyMailBox.TestForMessage()) {
			if (MyMailBox.GetMessageA() == TERMINATION_CODE) {
				break;
			}
		}
		Elevator2Status = Monitor2.GetElevatorStatusConsumer1();
		//PrintSync.Wait();
		//std::cout << "Elevator 2 Status " << Elevator2Status << endl;
		//PrintSync.Signal();
		drawfloorNumbers(2, Elevator2Status);
	}
	return 0;
}

int main() {
	setConsoleSize();
	MOVE_CURSOR(50, 1);
	cout << "Elevator 1 things" << endl;
	MOVE_CURSOR(80, 1);
	cout << "Elevator 2 things" << endl;
	drawFloors();

	for (int i = 0; i < 10; i++) {
		CCondition Name(string(string("Elevator1UpConditionF") + string(to_string(i))));
	}
	for (int i = 0; i < 10; i++) {
		CCondition Name(string(string("Elevator1DownConditionF") + string(to_string(i))));
	}
	for (int i = 0; i < 10; i++) {
		CCondition Name(string(string("Elevator2UpConditionF") + string(to_string(i))));
	}
	for (int i = 0; i < 10; i++) {
		CCondition Name(string(string("Elevator2DownConditionF") + string(to_string(i))));
	}
	int Elevator1UpUpdateFloorFlag = 0;
	int Elevator1DownUpdateFloorFlag = 0;
	int Elevator2UpUpdateFloorFlag = 0;
	int Elevator2DownUpdateFloorFlag = 0;

	CTypedPipe<int> PipeFromIo("Pipe", 1024);
	Monitor Monitor1("Monitor1");
	Monitor Monitor2("Monitor2");
	int PipeData, IoData = 0;
	list<int> Elevator1UpRequests;
	list<int> Elevator1DownRequests;
	list<int> Elevator2UpRequests;
	list<int> Elevator2DownRequests;
	list<int> RequestQueueUp;
	list<int> RequestQueueDown;
	list<int> Elevator1UpRequestsInside;
	list<int> Elevator1DownRequestsInside;
	list<int> Elevator2UpRequestsInside;
	list<int> Elevator2DownRequestsInside;

	CThread IoThread(IoThread, ACTIVE);
	CThread Elevator1Thread1(Elevator1Thread, ACTIVE);
	CThread Elevator2Thread1(Elevator2Thread, ACTIVE);
	CThread Elevator1StatusThread(Elevator1StatusThread, ACTIVE);
	CThread Elevator2StatusThread(Elevator2StatusThread, ACTIVE);

	int Elevator1Fault = 0;
	int Elevator2Fault = 0;

	r1.Wait();

	while (1) {

		if (PipeFromIo.TestForData() >= 1) {
			PipeFromIo.Read(&PipeData);
			IoData = PipeData;
			//PrintSync.Wait();
			//printf("This is the data recieved from IO Pipe: %d\n", PipeData);
			//PrintSync.Signal();
			/////////////////////////////////////////////////////////////////////////////////////////////////
			PrintSync.Wait();
			MOVE_CURSOR(10, 32);
			std::cout << "This is the IO data: " << IoData << "                  " << endl;
			PrintSync.Signal();
		}
		if (IoData == TERMINATION_CODE) {
			Elevator1Thread1.Post(TERMINATION_CODE);
			Elevator2Thread1.Post(TERMINATION_CODE);
			Elevator1StatusThread.Post(TERMINATION_CODE);
			Elevator2StatusThread.Post(TERMINATION_CODE);
			break;
		}

		if (IoData == ELEVATOR_1_CREATE_FAULT) {
			Elevator1Fault = 1;
			ElevatorsWorksorBroken(1, 0);
			IoData = 0;
			if (Elevator1UpRequests.size() > 0) {
				for (std::list<int>::iterator it = Elevator1UpRequests.begin(); it != Elevator1UpRequests.end(); ++it) {
					clearUpDown_RequestsOutside(*it, 11);
				}
				Elevator1UpRequests.clear();
			}
			if (Elevator1DownRequests.size() > 0) {
				for (std::list<int>::iterator it = Elevator1DownRequests.begin(); it != Elevator1DownRequests.end(); ++it) {
					clearUpDown_RequestsOutside(*it, 10);
				}
				Elevator1DownRequests.clear();
			}
			if (Elevator1UpRequestsInside.size()>0) {
				Elevator1UpRequestsInside.clear();
			}
			if (Elevator1DownRequestsInside.size() > 0) {
				Elevator1DownRequestsInside.clear();
			}
			clearGoingUpDown(1, 11);
			clearGoingUpDown(1, 10);
		}

		if (IoData == ELEVATOR_1_FIX_FAULT) {
			Elevator1Fault = 0;
			ElevatorsWorksorBroken(1, 1);
			IoData = 0;
			if (Elevator1UpRequests.size() > 0) {
				Elevator1UpRequests.clear();
			}
			if (Elevator1DownRequests.size() > 0) {
				Elevator1DownRequests.clear();
			}
			if (Elevator1UpRequestsInside.size() > 0) {
				Elevator1UpRequestsInside.clear();
			}
			if (Elevator1DownRequestsInside.size() > 0) {
				Elevator1DownRequestsInside.clear();
			}
			Monitor1.SetElevatorStatus(ELEVATOR_1_INITIAL_FLOOR);
			doorOpenClose(1, ELEVATOR_DOOR_OPEN);
		}

		if (IoData == ELEVATOR_2_CREATE_FAULT) {
			Elevator2Fault = 1;
			ElevatorsWorksorBroken(2, 0);
			IoData = 0;
			if (Elevator2UpRequests.size() > 0) {
				for (std::list<int>::iterator it = Elevator2UpRequests.begin(); it != Elevator2UpRequests.end(); ++it) {
					clearUpDown_RequestsOutside(*it, 11);
				}
				Elevator2UpRequests.clear();
			}
			if (Elevator2DownRequests.size() > 0) {
				for (std::list<int>::iterator it = Elevator2DownRequests.begin(); it != Elevator2DownRequests.end(); ++it) {
					clearUpDown_RequestsOutside(*it, 10);
				}
				Elevator2DownRequests.clear();
			}
			if (Elevator2UpRequestsInside.size() > 0) {
				Elevator2UpRequestsInside.clear();
			}
			if (Elevator2DownRequestsInside.size() > 0) {
				Elevator2DownRequestsInside.clear();
			}
			clearGoingUpDown(2, 11);
			clearGoingUpDown(2, 10);
		}

		if (IoData == ELEVATOR_2_FIX_FAULT) {
			Elevator2Fault = 0;
			ElevatorsWorksorBroken(2, 1);
			IoData = 0;
			if (Elevator2UpRequests.size() > 0) {
				Elevator2UpRequests.clear();
			}
			if (Elevator2DownRequests.size() > 0) {
				Elevator2DownRequests.clear();
			}
			if (Elevator2UpRequestsInside.size() > 0) {
				Elevator2UpRequestsInside.clear();
			}
			if (Elevator2DownRequestsInside.size() > 0) {
				Elevator2DownRequestsInside.clear();
			}
			Monitor2.SetElevatorStatus(ELEVATOR_2_INITIAL_FLOOR);
			doorOpenClose(2, ELEVATOR_DOOR_OPEN);
		}

		if (IoData != TERMINATION_CODE) {
			// Go up, someone is waiting for you outside the elevator
			if (IoData >= GOING_UP_MIN && IoData <= GOING_UP_MAX || RequestQueueUp.size() != 0) {
				int FloorToGo = IoData - GOING_UP_MIN;
				if (RequestQueueUp.size() != 0 && IoData >= GOING_UP_MIN) {
					RequestQueueUp.push_back(FloorToGo);
				}
				if (RequestQueueUp.size() != 0) {
					FloorToGo = RequestQueueUp.front();
					RequestQueueUp.pop_front();
				}
				int flag = 0;
				int flag_2 = 0;

				// Find out which elevator is closer
				int Elevator1Distance = FloorToGo - Elevator1Status;
				int Elevator2Distance = FloorToGo - Elevator2Status;

				if (Elevator1UpRequests.size() != 0) {
					if (FloorToGo < Elevator1UpRequests.front()) {
						flag = 1;
					}
					else {
						flag = 0;
					}

				}
				if (Elevator2UpRequests.size() != 0) {
					if (FloorToGo < Elevator2UpRequests.front()) {
						flag_2 = 1;
					}
					else {
						flag_2 = 0;
					}

				}

				// Give command to elevator which is closer
				if (Elevator1Fault == 0 && ((abs(Elevator1Distance) <= abs(Elevator2Distance) || Elevator2Fault == 1 || Elevator2DownRequests.size() != 0)) && (Elevator1Distance >= 0 || (Elevator1UpRequests.size() == 0)) && (Elevator1UpRequests.size() == 0 || flag == 1) && (Elevator1DownRequests.size() == 0) && Elevator1UpRequestsInside.size() == 0) {
					int DuplicateFlag = 0;
					if (Elevator2UpRequests.size() != 0) {
						for (std::list<int>::iterator it = Elevator2UpRequests.begin(); it != Elevator2UpRequests.end(); ++it) {
							if (*it == FloorToGo) {
								DuplicateFlag = 1;
							}
						}
					}
					if (!DuplicateFlag) {
						Elevator1UpRequests.push_back(FloorToGo);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
					}
				}
				else if (Elevator2Fault == 0 && (Elevator2Distance >= 0 || (Elevator2UpRequests.size() == 0)) && (Elevator2UpRequests.size() == 0 || flag_2 == 1) && Elevator2DownRequests.size() == 0 && Elevator2UpRequestsInside.size() == 0) {
					int DuplicateFlag = 0;
					if (Elevator1UpRequests.size() != 0) {
						for (std::list<int>::iterator it = Elevator1UpRequests.begin(); it != Elevator1UpRequests.end(); ++it) {
							if (*it == FloorToGo) {
								DuplicateFlag = 1;
							}

						}
					}
					if (!DuplicateFlag) {
						Elevator2UpRequests.push_back(FloorToGo);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
					}
				}
				else {
					RequestQueueUp.push_back(FloorToGo);
				}
				IoData = 0;
			}

			// Go down, someone is waiting for you outside the elevator
			if (IoData <= GOING_DOWN_MAX && IoData >= GOING_DOWN_MIN || RequestQueueDown.size() != 0) {
				int FloorToGo = IoData - GOING_DOWN_MIN;
				if (RequestQueueDown.size() != 0 && IoData >= GOING_DOWN_MIN && IoData <= GOING_DOWN_MAX) {
					RequestQueueDown.push_back(FloorToGo);

				}
				if (RequestQueueDown.size() != 0) {
					FloorToGo = RequestQueueDown.front();
					RequestQueueDown.pop_front();
				}

				int flag = 0;
				int flag_2 = 0;

				// Find out which elevator is closer
				int Elevator1Distance = FloorToGo - Elevator1Status;
				int Elevator2Distance = FloorToGo - Elevator2Status;

				if (Elevator1DownRequests.size() != 0) {
					if (FloorToGo > Elevator1DownRequests.front()) {
						flag = 1;
					}
					else {
						flag = 0;
					}

				}

				if (Elevator2DownRequests.size() != 0) {
					if (FloorToGo > Elevator2DownRequests.front()) {
						flag_2 = 1;
					}
					else {
						flag_2 = 0;
					}

				}

				// Give command to elevator which is closer
				if (Elevator1Fault == 0 && ((abs(Elevator1Distance) <= abs(Elevator2Distance) || Elevator2Fault == 1 || Elevator2UpRequests.size() != 0)) && (Elevator1Distance <= 0 || (Elevator1DownRequests.size() == 0)) && (Elevator1DownRequests.size() == 0 || flag == 1) && Elevator1UpRequests.size() == 0 && Elevator1DownRequestsInside.size() == 0) {
					int DuplicateFlag = 0;
					if (Elevator2DownRequests.size() != 0) {
						for (std::list<int>::iterator it = Elevator2DownRequests.begin(); it != Elevator2DownRequests.end(); ++it) {
							if (*it == FloorToGo) {
								DuplicateFlag = 1;
							}
						}
					}
					if (!DuplicateFlag) {
						Elevator1DownRequests.push_back(FloorToGo);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
					}
				}
				else if (Elevator2Fault == 0 && (Elevator2Distance <= 0 || Elevator2DownRequests.size() == 0) && (Elevator2DownRequests.size() == 0 || flag_2 == 1) && Elevator2UpRequests.size() == 0 && Elevator2DownRequestsInside.size() == 0) {
					int DuplicateFlag = 0;
					if (Elevator1DownRequests.size() != 0) {
						for (std::list<int>::iterator it = Elevator1DownRequests.begin(); it != Elevator1DownRequests.end(); ++it) {
							if (*it == FloorToGo) {
								DuplicateFlag = 1;
							}
						}
					}
					if (!DuplicateFlag) {
						Elevator2DownRequests.push_back(FloorToGo);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
					}
				}
				else {
					RequestQueueDown.push_back(FloorToGo);
				}
				IoData = 0;
			}

			if (Elevator1Fault == 0 && Elevator1UpRequests.size() > 0 && Elevator1Status >= 0 && !(IoData >= ELEVATOR_1_COMMAND_MIN && IoData <= ELEVATOR_1_COMMAND_MAX)) {
				Elevator1UpUpdateFloorFlag = 0;
				int flag = 0;
				for (std::list<int>::iterator it = Elevator1UpRequests.begin(); it != Elevator1UpRequests.end(); it++) {
					int Variable = abs(*it - Elevator1Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 1 doors at floor to go up " << Elevator1Status << endl;
						//PrintSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_OPEN);
						clearUpDown_RequestsOutside(Elevator1Status, 11);
						
						// Send message to Io thread and tell it that you are opening the doors
						Sleep(300);
						IoThread.Post(100);

						Elevator1UpUpdateFloorFlag = 1;
						// Going Up
						Elevator1DirectionProtectionSync.Wait();
						if (Elevator1Direction.size() == 0) {
							Elevator1Direction.push_back(MOVING_UP);
						}
						else {
							Elevator1Direction.pop_back();
							Elevator1Direction.push_back(MOVING_UP);
						}
						Elevator1DirectionProtectionSync.Signal();

						// Try and add a timeout here
						// If brand new flag is 1 that means active class is on
						// If brand new flag is 0 that means inputs are from characters/keyboard

						BrandNewFlagSync.Wait();
						if (!brand_new_flag) {
							BrandNewFlagSync.Signal();
							while (!major_flag) {
							
							}
						}
						else if (brand_new_flag == 1) {
							BrandNewFlagSync.Signal();
							CSemaphore Semaphore(string(string("Elevator1UpConditionF") + string(to_string(Elevator1Status)) + string("Sem")), 0, 1);
							Semaphore.Wait();
						}
						

						// Make this variable for every floor there should be a new condition
						CCondition Name(string(string("Elevator1UpConditionF") + string(to_string(Elevator1Status)) + string("Cond")));

						Name.Signal();
						Sleep(WAITING_TIME_FOR_PASSENGERS_TO_PRESS_BUTTON_FROM_INSIDE);
						Name.Reset();

						while (PipeFromIo.TestForData() == 0) {}

						if (it == Elevator1UpRequests.end()) {
							Elevator1UpRequests.pop_back();
							break;
						}
						else if (it == Elevator1UpRequests.begin())
						{
							Elevator1UpRequests.pop_front();
							break;
						}
						else if (Elevator1UpRequests.size() != 1) {
							it--;
							Elevator1UpRequests.remove(Elevator1Status);
							break;
						}
						else
						{
							Elevator1UpRequests.pop_back();
							break;
						}

					}

				}

				if (Elevator1UpRequestsInside.size() != 0) {
					for (std::list<int>::iterator it = Elevator1UpRequestsInside.begin(); it != Elevator1UpRequestsInside.end(); it++) {
						if (*it == Elevator1Status) {
							flag = 1;
						}
					}
				}

				if (Elevator1UpRequests.size() > 0 && Elevator1UpUpdateFloorFlag == 0 && flag == 0) {
					int Elevator1Distance = Elevator1UpRequests.front() - Elevator1Status;
					if (Elevator1Distance >= 1) {
						Elevator1UpStatusSync.Signal();
						Elevator1Thread1.Post(INCREMENT_FLOOR);
						Elevator1UpStatusSync1.Wait();
					}
					else if (Elevator1Distance <= -1) {
						Elevator1DownStatusSync.Signal();
						Elevator1Thread1.Post(DECREMENT_FLOOR);
						Elevator1DownStatusSync1.Wait();
					}
					//doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
				}
			}

			if (Elevator1Fault == 0 && Elevator1DownRequests.size() > 0 && Elevator1Status >= 0 && !(IoData >= ELEVATOR_1_COMMAND_MIN && IoData <= ELEVATOR_1_COMMAND_MAX)) {
				Elevator1DownUpdateFloorFlag = 0;
				int flag = 0;
				for (std::list<int>::iterator it = Elevator1DownRequests.begin(); it != Elevator1DownRequests.end(); it++) {
					int Variable = abs(*it - Elevator1Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 1 doors at floor to go down " << Elevator1Status << endl;
						//PrintSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_OPEN);
						clearUpDown_RequestsOutside(Elevator1Status, 10);

						// Send message to Io thread and tell it that you are opening the doors
						Sleep(300);
						IoThread.Post(100);

						Elevator1DownUpdateFloorFlag = 1;
						// Going down
						Elevator1DirectionProtectionSync.Wait();
						if (Elevator1Direction.size() == 0) {
							Elevator1Direction.push_back(MOVING_DOWN);
						}
						else {
							Elevator1Direction.pop_back();
							Elevator1Direction.push_back(MOVING_DOWN);
						}
						Elevator1DirectionProtectionSync.Signal();


						BrandNewFlagSync.Wait();
						if (!brand_new_flag) {
							BrandNewFlagSync.Signal();
							while (!major_flag) {
								//break;
							}
						}
						else if (brand_new_flag == 1) {
							BrandNewFlagSync.Signal();
							CSemaphore Semaphore(string(string("Elevator1DownConditionF") + string(to_string(Elevator1Status)) + string("Sem")), 0, 1);
							Semaphore.Wait();
						}
						

						// Make this variable for every floor there should be a new condition
						CCondition Name(string(string("Elevator1DownConditionF") + string(to_string(Elevator1Status)) + string("Cond")));

						Name.Signal();
						Sleep(WAITING_TIME_FOR_PASSENGERS_TO_PRESS_BUTTON_FROM_INSIDE);
						Name.Reset();


						while (PipeFromIo.TestForData() == 0) {}
						if (it == Elevator1DownRequests.end()) {
							Elevator1DownRequests.pop_back();
							break;
						}
						else if (it == Elevator1DownRequests.begin())
						{
							Elevator1DownRequests.pop_front();
							break;
						}
						else if (Elevator1DownRequests.size() != 1) {
							it--;
							Elevator1DownRequests.remove(Elevator1Status);
							break;
						}
						else
						{
							Elevator1DownRequests.pop_back();
							break;
						}

					}

				}

				if (Elevator1DownRequestsInside.size() != 0) {
					for (std::list<int>::iterator it = Elevator1DownRequestsInside.begin(); it != Elevator1DownRequestsInside.end(); it++) {
						if (*it == Elevator1Status) {
							flag = 1;
						}
					}
				}

				if (Elevator1DownRequests.size() > 0 && Elevator1DownUpdateFloorFlag == 0 && flag == 0) {
					int Elevator1Distance = Elevator1DownRequests.front() - Elevator1Status;

					if (Elevator1Distance >= 1) {
						Elevator1UpStatusSync.Signal();
						Elevator1Thread1.Post(INCREMENT_FLOOR);
						Elevator1UpStatusSync1.Wait();
					}
					if (Elevator1Distance <= -1) {
						Elevator1DownStatusSync.Signal();
						Elevator1Thread1.Post(DECREMENT_FLOOR);
						Elevator1DownStatusSync1.Wait();
					}
					//doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
				}

			}

			if (IoData >= ELEVATOR_1_COMMAND_MIN && IoData <= ELEVATOR_1_COMMAND_MAX) {
				int FloorToGo = IoData - ELEVATOR_1_COMMAND_MIN;
				int DuplicateFlag = 0;
				int DuplicateFlag_2 = 0;
				int DuplicateFlag_3 = 0;
				int DuplicateFlag_4 = 0;

				if (Elevator1DownRequestsInside.size() == 0 && Elevator1Direction.back() == 0) {
					for (std::list<int>::iterator it = Elevator1UpRequests.begin(); it != Elevator1UpRequests.end(); it++) {
						if (*it == FloorToGo) {
							DuplicateFlag = 1;
						}
					}
					if (Elevator1UpRequestsInside.size() != 0) {
						for (std::list<int>::iterator it = Elevator1UpRequestsInside.begin(); it != Elevator1UpRequestsInside.end(); it++) {
							if (*it == FloorToGo) {
								DuplicateFlag_2 = 1;
							}
						}
					}
					if (FloorToGo == Elevator1Status) {
						DuplicateFlag = 1;
					}
					if (!DuplicateFlag && !DuplicateFlag_2 && FloorToGo > Elevator1Status) {
						Elevator1UpRequestsInside.push_back(FloorToGo);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
					}
					

				}

				if (Elevator1UpRequestsInside.size() == 0 && Elevator1Direction.back() == 1) {
					for (std::list<int>::iterator it = Elevator1DownRequests.begin(); it != Elevator1DownRequests.end(); it++) {
						if (*it == FloorToGo) {
							DuplicateFlag_3 = 1;
						}
					}
					if (Elevator1DownRequestsInside.size() != 0) {
						for (std::list<int>::iterator it = Elevator1DownRequestsInside.begin(); it != Elevator1DownRequestsInside.end(); it++) {
							if (*it == FloorToGo) {
								DuplicateFlag_4 = 1;
							}
						}
					}
					if (FloorToGo == Elevator1Status) {
						DuplicateFlag_3 = 1;
					}
					if (!DuplicateFlag_3 && !DuplicateFlag_4 && FloorToGo < Elevator1Status) {
						Elevator1DownRequestsInside.push_back(FloorToGo);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
					}
					
				}
				IoData = 0;
			}

			if (Elevator1Fault == 0 && Elevator1UpRequestsInside.size() > 0 && Elevator1Status >= 0 && !(IoData >= ELEVATOR_1_COMMAND_MIN && IoData <= ELEVATOR_1_COMMAND_MAX)) {
				int flag = 0;
				for (std::list<int>::iterator it = Elevator1UpRequestsInside.begin(); it != Elevator1UpRequestsInside.end(); it++) {
					int Variable = abs(*it - Elevator1Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 1 doors so that the passenger can get off at " << Elevator1Status << endl;
						//PrintSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_OPEN);
						//clearUpDown_RequestsOutside(Elevator1Status, 11);
						clearGoingUpDown(1, 11);
						Sleep(GET_OFF_TIME);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);

						if (it == Elevator1UpRequestsInside.end()) {
							Elevator1UpRequestsInside.pop_back();
							break;
						}
						else if (it == Elevator1UpRequestsInside.begin())
						{
							Elevator1UpRequestsInside.pop_front();
							break;
						}
						else if (Elevator1UpRequestsInside.size() != 1) {
							it--;
							Elevator1UpRequestsInside.remove(Elevator1Status);
							break;
						}
						else
						{
							Elevator1UpRequestsInside.pop_back();
							break;
						}

					}

				}
				if (Elevator1UpRequests.size() != 0) {
					for (std::list<int>::iterator it = Elevator1UpRequests.begin(); it != Elevator1UpRequests.end(); it++) {
						if (*it == Elevator1Status) {
							flag = 1;
						}
					}
				}

				if (Elevator1UpRequestsInside.size() > 0 && flag == 0 && Elevator1DownRequests.size() == 0) {
					int Elevator1Distance = Elevator1UpRequestsInside.front() - Elevator1Status;
					if (Elevator1Distance >= 1) {
						Elevator1UpStatusSync.Signal();
						Elevator1Thread1.Post(INCREMENT_FLOOR);
						Elevator1UpStatusSync1.Wait();
					}
					else if (Elevator1Distance <= -1) {
						Elevator1DownStatusSync.Signal();
						Elevator1Thread1.Post(DECREMENT_FLOOR);
						Elevator1DownStatusSync1.Wait();
					}
					//doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
				}
				PrintSync.Wait();
				MOVE_CURSOR(0, 19);
				cout << "Number of people in elevator 1 is " << Elevator1UpRequestsInside.size() << "         ";
				PrintSync.Signal();
			}

			if (Elevator1Fault == 0 && Elevator1DownRequestsInside.size() > 0 && Elevator1Status >= 0 && !(IoData >= ELEVATOR_1_COMMAND_MIN && IoData <= ELEVATOR_1_COMMAND_MAX)) {
				int flag = 0;
				for (std::list<int>::iterator it = Elevator1DownRequestsInside.begin(); it != Elevator1DownRequestsInside.end(); it++) {
					int Variable = abs(*it - Elevator1Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 1 doors so that the passenger can get off at " << Elevator1Status << endl;
						//PrintSync.Signal();
						doorOpenClose(1, ELEVATOR_DOOR_OPEN);
						//clearUpDown_RequestsOutside(Elevator1Status, 10);
						clearGoingUpDown(1, 10);
						Sleep(GET_OFF_TIME);
						doorOpenClose(1, ELEVATOR_DOOR_CLOSE);

						if (it == Elevator1DownRequestsInside.end()) {
							Elevator1DownRequestsInside.pop_back();
							break;
						}
						else if (it == Elevator1DownRequestsInside.begin())
						{
							Elevator1DownRequestsInside.pop_front();
							break;
						}
						else if (Elevator1DownRequestsInside.size() != 1) {
							it--;
							Elevator1DownRequestsInside.remove(Elevator1Status);
							break;
						}
						else
						{
							Elevator1DownRequestsInside.pop_back();
							break;
						}

					}

				}
				if (Elevator1DownRequests.size() != 0) {
					for (std::list<int>::iterator it = Elevator1DownRequests.begin(); it != Elevator1DownRequests.end(); it++) {
						if (*it == Elevator1Status) {
							flag = 1;
						}
					}
				}

				if (Elevator1DownRequestsInside.size() > 0 && flag == 0 && Elevator1UpRequests.size() == 0) {
					int Elevator1Distance = Elevator1DownRequestsInside.front() - Elevator1Status;
					if (Elevator1Distance >= 1) {
						Elevator1UpStatusSync.Signal();
						Elevator1Thread1.Post(INCREMENT_FLOOR);
						Elevator1UpStatusSync1.Wait();
					}
					else if (Elevator1Distance <= -1) {
						Elevator1DownStatusSync.Signal();
						Elevator1Thread1.Post(DECREMENT_FLOOR);
						Elevator1DownStatusSync1.Wait();
					}
					//doorOpenClose(1, ELEVATOR_DOOR_CLOSE);
				}
				PrintSync.Wait();
				MOVE_CURSOR(0, 19);
				cout << "Number of people in elevator 1 is " << Elevator1DownRequestsInside.size() << "         ";
				PrintSync.Signal();
			}

			if (Elevator2Fault == 0 && Elevator2UpRequests.size() > 0 && Elevator2Status >= 0 && !(IoData >= ELEVATOR_2_COMMAND_MIN && IoData <= ELEVATOR_2_COMMAND_MAX)) {
				Elevator2UpUpdateFloorFlag = 0;
				int flag = 0;
				for (std::list<int>::iterator it = Elevator2UpRequests.begin(); it != Elevator2UpRequests.end(); it++) {
					int Variable = abs(*it - Elevator2Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 2 doors at floor to go up " << Elevator2Status << endl;
						//PrintSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_OPEN);
						clearUpDown_RequestsOutside(Elevator2Status, 11);

						// Send message to Io thread and tell it that you are opening the doors
						Sleep(300);
						IoThread.Post(100);

						Elevator2UpUpdateFloorFlag = 1;
						// Going Up
						Elevator2DirectionProtectionSync.Wait();
						if (Elevator2Direction.size() == 0) {
							Elevator2Direction.push_back(MOVING_UP);
						}
						else {
							Elevator2Direction.pop_back();
							Elevator2Direction.push_back(MOVING_UP);
						}
						Elevator2DirectionProtectionSync.Signal();

						BrandNewFlagSync.Wait();
						if (!brand_new_flag) {
							BrandNewFlagSync.Signal();
							while (!major_flag) {
								//break;
							}
						}
						else if (brand_new_flag == 1) {
							BrandNewFlagSync.Signal();
							CSemaphore Semaphore(string(string("Elevator2UpConditionF") + string(to_string(Elevator2Status)) + string("Sem")), 0, 1);
							Semaphore.Wait();
						}
						

						// Make this variable for every floor there should be a new condition
						CCondition Name(string(string("Elevator2UpConditionF") + string(to_string(Elevator2Status)) + string("Cond")));

						Name.Signal();
						Sleep(WAITING_TIME_FOR_PASSENGERS_TO_PRESS_BUTTON_FROM_INSIDE);
						Name.Reset();

						while (PipeFromIo.TestForData() == 0) {}

						if (it == Elevator2UpRequests.end()) {
							Elevator2UpRequests.pop_back();
							break;
						}
						else if (it == Elevator2UpRequests.begin())
						{
							Elevator2UpRequests.pop_front();
							break;
						}
						else if (Elevator2UpRequests.size() != 1) {
							it--;
							Elevator2UpRequests.remove(Elevator2Status);
							break;
						}
						else
						{
							Elevator2UpRequests.pop_back();
							break;
						}

					}

				}

				if (Elevator2UpRequestsInside.size() != 0) {
					for (std::list<int>::iterator it = Elevator2UpRequestsInside.begin(); it != Elevator2UpRequestsInside.end(); it++) {
						if (*it == Elevator2Status) {
							flag = 1;
						}
					}
				}

				if (Elevator2UpRequests.size() > 0 && Elevator2UpUpdateFloorFlag == 0 && flag == 0) {
					int Elevator2Distance = Elevator2UpRequests.front() - Elevator2Status;

					if (Elevator2Distance >= 1) {
						Elevator2UpStatusSync.Signal();
						Elevator2Thread1.Post(INCREMENT_FLOOR);
						Elevator2UpStatusSync1.Wait();
					}
					else if (Elevator2Distance <= -1) {
						Elevator2DownStatusSync.Signal();
						Elevator2Thread1.Post(DECREMENT_FLOOR);
						Elevator2DownStatusSync1.Wait();
					}
					//doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
				}
			}

			if (Elevator2Fault == 0 && Elevator2DownRequests.size() > 0 && Elevator2Status >= 0 && !(IoData >= ELEVATOR_2_COMMAND_MIN && IoData <= ELEVATOR_2_COMMAND_MAX)) {
				Elevator2DownUpdateFloorFlag = 0;
				int flag = 0;
				for (std::list<int>::iterator it = Elevator2DownRequests.begin(); it != Elevator2DownRequests.end(); it++) {
					int Variable = abs(*it - Elevator2Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 2 doors at floor to go down " << Elevator2Status << endl;
						//PrintSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_OPEN);
						clearUpDown_RequestsOutside(Elevator2Status, 10);

						// Send message to Io thread and tell it that you are opening the doors
						Sleep(300);
						IoThread.Post(100);

						Elevator2DownUpdateFloorFlag = 1;
						// Going down
						Elevator2DirectionProtectionSync.Wait();
						if (Elevator2Direction.size() == 0) {
							Elevator2Direction.push_back(MOVING_DOWN);
						}
						else {
							Elevator2Direction.pop_back();
							Elevator2Direction.push_back(MOVING_DOWN);
						}
						Elevator2DirectionProtectionSync.Signal();

						BrandNewFlagSync.Wait();
						if (!brand_new_flag) {
							BrandNewFlagSync.Signal();
							while (!major_flag) {
								//break;
							}
						}
						else if (brand_new_flag == 1) {
							BrandNewFlagSync.Signal();
							CSemaphore Semaphore(string(string("Elevator2DownConditionF") + string(to_string(Elevator2Status)) + string("Sem")), 0, 1);
							Semaphore.Wait();
						}


						// Make this variable for every floor there should be a new condition
						CCondition Name(string(string("Elevator2DownConditionF") + string(to_string(Elevator2Status)) + string("Cond")));

						Name.Signal();
						Sleep(WAITING_TIME_FOR_PASSENGERS_TO_PRESS_BUTTON_FROM_INSIDE);
						Name.Reset();


						while (PipeFromIo.TestForData() == 0) {}
						if (it == Elevator2DownRequests.end()) {
							Elevator2DownRequests.pop_back();
							break;
						}
						else if (it == Elevator2DownRequests.begin())
						{
							Elevator2DownRequests.pop_front();
							break;
						}
						else if (Elevator2DownRequests.size() != 1) {
							it--;
							Elevator2DownRequests.remove(Elevator2Status);
							break;
						}
						else
						{
							Elevator2DownRequests.pop_back();
							break;
						}

					}

				}

				if (Elevator2DownRequestsInside.size() != 0 && Elevator2DownUpdateFloorFlag == 0 && flag == 0) {
					for (std::list<int>::iterator it = Elevator2DownRequestsInside.begin(); it != Elevator2DownRequestsInside.end(); it++) {
						if (*it == Elevator2Status) {
							flag = 1;
						}
					}
				}

				if (Elevator2DownRequests.size() > 0 && Elevator2DownUpdateFloorFlag == 0 && flag == 0) {
					int Elevator2Distance = Elevator2DownRequests.front() - Elevator2Status;

					if (Elevator2Distance >= 1) {
						Elevator2UpStatusSync.Signal();
						Elevator2Thread1.Post(INCREMENT_FLOOR);
						Elevator2UpStatusSync1.Wait();
					}
					if (Elevator2Distance <= -1) {
						Elevator2DownStatusSync.Signal();
						Elevator2Thread1.Post(DECREMENT_FLOOR);
						Elevator2DownStatusSync1.Wait();
					}
					//doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
				}
			}

			if (IoData >= ELEVATOR_2_COMMAND_MIN && IoData <= ELEVATOR_2_COMMAND_MAX) {
				int FloorToGo = IoData - ELEVATOR_2_COMMAND_MIN;
				int DuplicateFlag = 0;
				int DuplicateFlag_2 = 0;
				int DuplicateFlag_3 = 0;
				int DuplicateFlag_4 = 0;

				if (Elevator2DownRequestsInside.size() == 0 && Elevator2Direction.back() == 0) {
					for (std::list<int>::iterator it = Elevator2UpRequests.begin(); it != Elevator2UpRequests.end(); it++) {
						if (*it == FloorToGo) {
							DuplicateFlag = 1;
						}
					}
					if (Elevator2UpRequestsInside.size() != 0) {
						for (std::list<int>::iterator it = Elevator2UpRequestsInside.begin(); it != Elevator2UpRequestsInside.end(); it++) {
							if (*it == FloorToGo) {
								DuplicateFlag_2 = 1;
							}
						}
					}
					if (FloorToGo == Elevator2Status) {
						DuplicateFlag = 1;
					}
					if (!DuplicateFlag && !DuplicateFlag_2 && FloorToGo > Elevator2Status) {
						Elevator2UpRequestsInside.push_back(FloorToGo);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
					}
					

				}

				if (Elevator2UpRequestsInside.size() == 0 && Elevator2Direction.back() == 1) {
					for (std::list<int>::iterator it = Elevator2DownRequests.begin(); it != Elevator2DownRequests.end(); it++) {
						if (*it == FloorToGo) {
							DuplicateFlag_3 = 1;
						}
					}
					if (Elevator2DownRequestsInside.size() != 0) {
						for (std::list<int>::iterator it = Elevator2DownRequestsInside.begin(); it != Elevator2DownRequestsInside.end(); it++) {
							if (*it == FloorToGo) {
								DuplicateFlag_4 = 1;
							}
						}
					}
					if (FloorToGo == Elevator2Status) {
						DuplicateFlag_3 = 1;
					}
					if (!DuplicateFlag_3 && !DuplicateFlag_4 && FloorToGo < Elevator2Status) {
						Elevator2DownRequestsInside.push_back(FloorToGo);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
					}
					
				}

				IoData = 0;
			}

			if (Elevator2Fault == 0 && Elevator2UpRequestsInside.size() > 0 && Elevator2Status >= 0 && !(IoData >= ELEVATOR_2_COMMAND_MIN && IoData <= ELEVATOR_2_COMMAND_MAX)) {
				int flag = 0;
				for (std::list<int>::iterator it = Elevator2UpRequestsInside.begin(); it != Elevator2UpRequestsInside.end(); it++) {
					int Variable = abs(*it - Elevator2Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 2 doors so that the passenger can get off at " << Elevator2Status << endl;
						//PrintSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_OPEN);
						//clearUpDown_RequestsOutside(Elevator2Status, 11);
						clearGoingUpDown(2, 11);
						Sleep(GET_OFF_TIME);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);

						if (it == Elevator2UpRequestsInside.end()) {
							Elevator2UpRequestsInside.pop_back();
							break;
						}
						else if (it == Elevator2UpRequestsInside.begin())
						{
							Elevator2UpRequestsInside.pop_front();
							break;
						}
						else if (Elevator2UpRequestsInside.size() != 1) {
							it--;
							Elevator2UpRequestsInside.remove(Elevator2Status);
							break;
						}
						else
						{
							Elevator2UpRequestsInside.pop_back();
							break;
						}

					}

				}
				if (Elevator2UpRequests.size() != 0) {
					for (std::list<int>::iterator it = Elevator2UpRequests.begin(); it != Elevator2UpRequests.end(); it++) {
						if (*it == Elevator2Status) {
							flag = 1;
						}
					}
				}

				if (Elevator2UpRequestsInside.size() > 0 && flag == 0 && Elevator2DownRequests.size() == 0) {
					int Elevator2Distance = Elevator2UpRequestsInside.front() - Elevator2Status;
					if (Elevator2Distance >= 1) {
						Elevator2UpStatusSync.Signal();
						Elevator2Thread1.Post(INCREMENT_FLOOR);
						Elevator2UpStatusSync1.Wait();
					}
					else if (Elevator2Distance <= -1) {
						Elevator2DownStatusSync.Signal();
						Elevator2Thread1.Post(DECREMENT_FLOOR);
						Elevator2DownStatusSync1.Wait();
					}
					//doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
				}
				PrintSync.Wait();
				MOVE_CURSOR(0, 20);
				cout << "Number of people in elevator 2 is " << Elevator2UpRequestsInside.size() << "         ";
				PrintSync.Signal();
			}

			if (Elevator2Fault == 0 && Elevator2DownRequestsInside.size() > 0 && Elevator2Status >= 0 && !(IoData >= ELEVATOR_2_COMMAND_MIN && IoData <= ELEVATOR_2_COMMAND_MAX)) {
				int flag = 0;
				for (std::list<int>::iterator it = Elevator2DownRequestsInside.begin(); it != Elevator2DownRequestsInside.end(); it++) {
					int Variable = abs(*it - Elevator2Status);
					if (Variable == 0) {
						//PrintSync.Wait();
						//std::cout << "Open elevator 2 doors so that the passenger can get off at " << Elevator2Status << endl;
						//PrintSync.Signal();
						doorOpenClose(2, ELEVATOR_DOOR_OPEN);
						//clearUpDown_RequestsOutside(Elevator2Status, 10);
						clearGoingUpDown(2, 10);
						Sleep(GET_OFF_TIME);
						doorOpenClose(2, ELEVATOR_DOOR_CLOSE);

						if (it == Elevator2DownRequestsInside.end()) {
							Elevator2DownRequestsInside.pop_back();
							break;
						}
						else if (it == Elevator2DownRequestsInside.begin())
						{
							Elevator2DownRequestsInside.pop_front();
							break;
						}
						else if (Elevator2DownRequestsInside.size() != 1) {
							it--;
							Elevator2DownRequestsInside.remove(Elevator2Status);
							break;
						}
						else
						{
							Elevator2DownRequestsInside.pop_back();
							break;
						}

					}

				}
				if (Elevator2DownRequests.size() != 0) {
					for (std::list<int>::iterator it = Elevator2DownRequests.begin(); it != Elevator2DownRequests.end(); it++) {
						if (*it == Elevator2Status) {
							flag = 1;
						}
					}
				}

				if (Elevator2DownRequestsInside.size() > 0 && flag == 0 && Elevator2UpRequests.size() == 0) {
					int Elevator2Distance = Elevator2DownRequestsInside.front() - Elevator2Status;
					if (Elevator2Distance >= 1) {
						Elevator2UpStatusSync.Signal();
						Elevator2Thread1.Post(INCREMENT_FLOOR);
						Elevator2UpStatusSync1.Wait();
					}
					else if (Elevator2Distance <= -1) {
						Elevator2DownStatusSync.Signal();
						Elevator2Thread1.Post(DECREMENT_FLOOR);
						Elevator2DownStatusSync1.Wait();
					}
					//doorOpenClose(2, ELEVATOR_DOOR_CLOSE);
				}
				PrintSync.Wait();
				MOVE_CURSOR(0, 20);
				cout << "Number of people in elevator 2 is " << Elevator2DownRequestsInside.size() << "         ";
				PrintSync.Signal();
			}

		}
	}

	PrintSync.Wait();
	MOVE_CURSOR(10, 30);
	std::cout << "Simulation Terminated" << endl;
	PrintSync.Signal();

	return 0;
}

