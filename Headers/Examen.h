// If the file hasn't been include 
#ifndef function
// You can include the file
#define function
	
	int MillisecondsToSeconds(int ms);
	int SecondsToMinutes(int secs);
	struct Car;
	struct Car CarBuilder(int arrayId[]);
	void PrintCarTurnTiming(struct Car car);
	int* BestTimeFromMS(int ms);
	int MinutesToHours(int min);

#endif
