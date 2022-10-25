// If the file hasn't been include 
#ifndef functionsCars

	// You can include the file
	#define functionsCars
		
	typedef struct Car Car;
	Car *CarBuilder(int arrayId[], int size);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoRace(Car *car, int minutes, Car *shMem);
	void InitialisationCar(Car *car);
	float MSToSeconds(int MS);
	void WriteInSharedMemory(Car *shMem, Car *car);
	int RandomNumber(int min, int max);
	void EndOfProgramParent();
	void EndOfProgramChild();
	
	// Global variables
	const char *semaChildName = "/child", *semaParentName = "/parent";
	sem_t *semaChildId, *semaParentId;
	Car *shMem;
	int shmId;
	
	#define NUMBEROFCARS 20
	#define NUMBEROFCARSQ2 15
	#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/FunctionsCars.c"

#endif
