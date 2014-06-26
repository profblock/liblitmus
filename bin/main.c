#include <stdio.h>
#include <stdlib.h>
#include "feedback.h"
#include "whisper.h"
#include <math.h>

int main()
{
	int j;
	int i;
	int numberOfOperations;
	micSpeakerStruct* ms;
	//micSpeakerStruct* ms1;
	//OccludingPointsStruct* ops;

	//initWhisperRoom(3, 2, 8, 4, 1, 2000000, 800000, 1.2, 100000, 100);
	initWhisperRoom(3, 2, 8, 4, 0, 250, 2000, 500, .1, 100, 10);

	
	addNoise(1, 10, 3);
	addNoise(20, 30, 5);
	//ms1 = constructSpeakerMicPairByNumber(2*8-1);
	for(j = 0;j<4*8;j++){
		 ms = constructSpeakerMicPairByNumber(j);
	}
	
	//ops = (OccludingPointsStruct*)malloc(sizeof(OccludingPointsStruct));

	for(i =0;i<4000;i++){
		updatePosition(ms, 1);
		numberOfOperations = getNumberOfOperations(ms);
		printf("%d\n", numberOfOperations); 
	}
	return 0;
}