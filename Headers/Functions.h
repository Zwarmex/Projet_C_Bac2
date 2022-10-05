// If the file hasn't been include 
#ifndef functions

	// You can include the file
	#define functions
		
	int MillisecondsToSeconds(int ms);
	int SecondsToMinutes(int secs);
	typedef struct Car Car;
	Car *CarBuilder(int arrayId[]);
	void PrintBestTimeWithText(int id, int timeMS, char sent[]);
	int MinutesToHours(int min);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoFreeTry(Car *car);
	void InitialisationOrResetCar(Car *car);
	
	#define NUMBEROFCARS 20
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/Functions.c"

#endif
