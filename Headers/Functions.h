// If the file hasn't been include 
#ifndef function
// You can include the file

#define function
	
	int MillisecondsToSeconds(int ms);
	int SecondsToMinutes(int secs);
	struct Car;
	struct Car *CarBuilder(int arrayId[]);
	void PrintBestTimeWithText(int id, int timeMS, char sent[]);
	int* BestTimeFromMS(int ms);
	int MinutesToHours(int min);
	struct Car EndOfSession(struct Car car);
	struct Car EnterThePits(struct Car car);
	void DoFreeTry(struct Car car);
	
	#define NUMBEROFCARS 20
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/Functions.c"

#endif
