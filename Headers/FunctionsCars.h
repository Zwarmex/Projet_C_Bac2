// If the file hasn't been included
#ifndef functionsCars

	// You can include the file
	#define functionsCars
		
	typedef struct Car Car;
	Car *CarBuilder(const int arrayId[], int size);
	void EndOfSession(Car *car);
	void EnterThePits(Car *car);
	void DoRace(Car *car, int minutes, Car *shMem, int numberOfTurnsMax, int boolRace);
	void InitialisationCar(Car *car);
	void WriteInSharedMemory(Car *shMem, Car *car);
	int RandomNumber(int min, int max);
	void EndOfProgramParent();
	void EndOfProgramChild();
	
	// Global variables
	const char *semaChildName = "/child", *semaParentName = "/parent";
	sem_t *semaChildId, *semaParentId;
	Car *shMem;
	int shmId;
	
	#define NUMBER_OF_CARS 20
	#define NUMBER_OF_CARS_Q2 15
	#define NUMBER_OF_CARS_Q3 10
    #define CAR_TIME_BUFFER 5
    #define BEST_TIME_BUFFER 4
	#define LENGTH_ARRAY(array) (sizeof(array)/sizeof(*array))
	#include "../Sources/FunctionsCars.c"

#endif
