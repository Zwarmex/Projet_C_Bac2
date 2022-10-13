// If the file hasn't been include 
#ifndef functions

	// You can include the file
	#define functions
		
	typedef struct Car Car;
	Car *CarBuilder(int arrayId[]);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoFreeTry(Car *car);
	void InitialisationOrResetCar(Car *car);
	Car *SelectionSort(Car *arrayCars);
	float MSToSeconds(int MS);
    void PrintScore(Car *arrayCars);
    char *returnBestTime(int timeMS, char *buff);
	
	#define NUMBEROFCARS 20
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/Functions.c"

#endif
