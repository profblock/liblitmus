#ifndef _FEEDBACK_H_
#define _FEEDBACK_H_
/* Feedback.h 
 * Created by Aaron Block
 * Includes all files necessary for estimating
 * Feedback values given previous inputs
 */
void printTestString();
int testNumber();

typedef struct taskStruct{
	int jobNumber;
	double pValue;
	double iValue;
	int totalError;
	int currentError;
} taskStruct;
 

#endif