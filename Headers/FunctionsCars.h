// If the file hasn't been include 
#ifndef functionsCars

	// You can include the file
	#define functionsCars
		
	typedef struct Car Car;
	Car *CarBuilder(int arrayId[]);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoFreeTry(Car *car);
	void InitialisationOrResetCar(Car *car);
	Car *SelectionSort(Car *arrayCars);
	float MSToSeconds(int MS);
	int RandomNumber(int min, int max);
	
	#define NUMBEROFCARS 20
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/FunctionsCars.c"

#endif
