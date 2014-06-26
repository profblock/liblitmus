#include "feedback.h"
#include <stdio.h>

void printTestString()
{
	int x= testNumber();
	int y = 15;
	printf("Test %d %d\n", x,y);
}

/* This formula comes formula (6.1) from my dissertation
 * pValue is the "a" value
 * iValue is the "b" value
 * currentError is the epsilon
 * totalError is the summation.
 * This returns the estimated weight fro the next Task
 */
 
void updateWeight(taskStruct * task, int actualWeight, int estimatedWeight)
{
	task->totalError +=	task->currentError;
	task->currentError = estimatedWeight-actualWeight;
	task->jobNumber++;
}

/* This formula comes formula (6.1) from my dissertation
 * pValue is the "a" value
 * iValue is the "b" value
 * currentError is the epsilon
 * totalError is the summation.
 * This returns the estimated weight fro the next Task
 */
int predictWeight(taskStruct* task){
	int returnValue = (int) (task->pValue * task->currentError + task->iValue*task->totalError);
	return returnValue;
}

int testNumber()
{
	return 4;
}	
