// If the file hasn't been include 
#ifndef functions

	// You can include the file
	#define functions
		
	typedef struct Car Car;
	Car *CarBuilder(int arrayId[]);
	char *PrintBestTimeWithText(int timeMS);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoFreeTry(Car *car);
	void InitialisationOrResetCar(Car *car);
	Car *SelectionSort(Car *arrayCars);
	float MSToSeconds(int MS);
	void PrintScore(Car *arrayCars);
	
	#define NUMBEROFCARS 20
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/Functions.c"

#endif