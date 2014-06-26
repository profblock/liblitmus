/* based_mt_task.c -- A basic multi-threaded real-time task skeleton. 
 *
 * This (by itself useless) task demos how to setup a multi-threaded LITMUS^RT
 * real-time task. Familiarity with the single threaded example (base_task.c)
 * is assumed.
 *
 * Currently, liblitmus still lacks automated support for real-time
 * tasks, but internaly it is thread-safe, and thus can be used together
 * with pthreads.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include gettid() */
#include <sys/types.h>

/* Include threading support. */
#include <pthread.h>

#include "feedback.h"
#include "whisper.h"
#include <math.h>

/* Include the LITMUS^RT API.*/
#include "litmus.h"

#define PERIOD            1000
#define RELATIVE_DEADLINE 10
#define EXEC_COST         10

#define R_ARRAY_SIZE_1	  101
#define R_ARRAY_SIZE_2	  103

/* Let's create 10 threads in the example, 
 * for a total utilization of 1.
 */
#define NUM_THREADS      32 

/* The information passed to each thread. Could be anything. */
struct thread_context {
	int id;
};

/* The real-time thread program. Doesn't have to be the same for
 * all threads. Here, we only have one that will invoke job().
 */
void* rt_thread(void *tcontext);

/* Declare the periodically invoked job. 
 * Returns 1 -> task should exit.
 *         0 -> task should continue.
 */
int job(int id, micSpeakerStruct* ms, double rArray1[], double rArray2[]);


/* Catch errors.
 */
#define CALL( exp ) do { \
		int ret; \
		ret = exp; \
		if (ret != 0) \
			fprintf(stderr, "%s failed: %m\n", #exp);\
		else \
			fprintf(stderr, "%s ok.\n", #exp); \
	} while (0)


/* Basic setup is the same as in the single-threaded example. However, 
 * we do some thread initiliazation first before invoking the job.
 */
int main(int argc, char** argv)
{
	//int j;
	int i;
	//int numberOfOperations;
	//micSpeakerStruct* ms;
	struct thread_context ctx[NUM_THREADS];
	pthread_t             task[NUM_THREADS];

	/* The task is in background mode upon startup. */		


	/*****
	 * 1) Command line paramter parsing would be done here.
	 */


       
	/*****
	 * 2) Work environment (e.g., global data structures, file data, etc.) would
	 *    be setup here.
	 */

	//micSpeakerStruct* ms1;
	//OccludingPointsStruct* ops;

	//initWhisperRoom(3, 2, 8, 4, 1, 2000000, 800000, 1.2, 100000, 100);
	initWhisperRoom(3, 2, 8, 4, 0, 250, 2000, 500, .1, 100, 10);

	
	addNoise(1, 10, 3);
	addNoise(20, 30, 5);
	//ms1 = constructSpeakerMicPairByNumber(2*8-1);
/*	for(j = 0;j<4*8;j++){
		 ms = constructSpeakerMicPairByNumber(j);
	}*/
	
	//ops = (OccludingPointsStruct*)malloc(sizeof(OccludingPointsStruct));



	/*****
	 * 3) Initialize LITMUS^RT.
	 *    Task parameters will be specified per thread.
	 */
	init_litmus();


	/***** 
	 * 4) Launch threads.
	 */
	for (i = 0; i < NUM_THREADS; i++) {
		ctx[i].id = i;
		pthread_create(task + i, NULL, rt_thread, (void *) (ctx + i));
	}

	
	/*****
	 * 5) Wait for RT threads to terminate.
	 */
	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(task[i], NULL);
	

	/***** 
	 * 6) Clean up, maybe print results and stats, and exit.
	 */
	return 0;
}



/* A real-time thread is very similar to the main function of a single-threaded
 * real-time app. Notice, that init_rt_thread() is called to initialized per-thread
 * data structures of the LITMUS^RT user space libary.
 */
void* rt_thread(void *tcontext)
{
	//int do_exit;
	micSpeakerStruct* ms;
	struct thread_context *ctx = (struct thread_context *) tcontext;
	struct rt_task param;
	double randomValues1[R_ARRAY_SIZE_1];
	double randomValues2[R_ARRAY_SIZE_2];
	int i;
	int k;
	
	for (i = 0; i < R_ARRAY_SIZE_1 ; i++) {
		randomValues1[i] = (rand()%5000)/5000.0;
	}
	
	for (i = 0; i < R_ARRAY_SIZE_2 ; i++) {
		randomValues2[i] = (rand()%5000)/2500.0-1;
	}
	
	ms = constructSpeakerMicPairByNumber(ctx->id);

	/* Set up task parameters */
	init_rt_task_param(&param);
	param.exec_cost = ms2ns(EXEC_COST);
	param.period = ms2ns(PERIOD);
	param.relative_deadline = ms2ns(RELATIVE_DEADLINE);

	/* What to do in the case of budget overruns? */
	param.budget_policy = NO_ENFORCEMENT;

	/* The task class parameter is ignored by most plugins. */
	param.cls = RT_CLASS_SOFT;

	/* The priority parameter is only used by fixed-priority plugins. */
	param.priority = LITMUS_LOWEST_PRIORITY;

	/* Make presence visible. */
	printf("RT Thread %d active.\n", ctx->id);
	
	
	
	param.service_levels =(struct rt_service_level*)malloc(sizeof(struct rt_service_level)*4);
	param.service_levels[0].relative_work = 1;
	param.service_levels[0].quality_of_service = 2.2;
	param.service_levels[0].service_level_number = 0;
	param.service_levels[0].service_level_period = ms2ns(PERIOD);

	param.service_levels[1].relative_work = 2;
	param.service_levels[1].quality_of_service = 3;
	param.service_levels[1].service_level_number = 1;
	param.service_levels[1].service_level_period = ms2ns(PERIOD);

	param.service_levels[2].relative_work = 2.1;
	param.service_levels[2].quality_of_service = 4;
	param.service_levels[2].service_level_number = 2;
	param.service_levels[2].service_level_period = ms2ns(PERIOD);

	param.service_levels[3].relative_work = 3;
	param.service_levels[3].quality_of_service = 5;
	param.service_levels[3].service_level_number = 3;
	param.service_levels[3].service_level_period = ms2ns(PERIOD);

	/*****
	 * 1) Initialize real-time settings.
	 */
	CALL( init_rt_thread() );

	/* To specify a partition, do
	 *
	 * param.cpu = CPU;
	 * be_migrate_to(CPU);
	 *
	 * where CPU ranges from 0 to "Number of CPUs" - 1 before calling
	 * set_rt_task_param().
	 */
	CALL( set_rt_task_param(gettid(), &param) );

	/*****
	 * 2) Transition to real-time mode.
	 */
	CALL( task_mode(LITMUS_RT_TASK) );

	/* The task is now executing as a real-time task if the call didn't fail. 
	 */



	/*****
	 * 3) Invoke real-time jobs.
	 */
	for(k=0;k<240;k++){
		/* Wait until the next job is released. */
		sleep_next_period();
		/* Invoke job. */
		job(ctx->id, ms, randomValues1, randomValues2);		
	}// while (!do_exit);


	
	/*****
	 * 4) Transition to background mode.
	 */
	CALL( task_mode(BACKGROUND_TASK) );


	return NULL;
}



int job(int id, micSpeakerStruct* ms, double rArray1[], double rArray2[]) 
{
	/* Do real-time calculation. */
	long int i =0;
	int rIndex1 = 0;
	int rIndex2 = 0;
	int total=0;
	int numberOfOperations;
	//struct control_page* myControlPage = get_ctrl_page();
	//unsigned int myServiceLevel = myControlPage->service_level;
	//printf("Service Level %u of thread %d\n",myServiceLevel, id);
	/* Don't exit. */
	
	updatePosition(ms, 1);
	//Increased the number of iterations 
	//TODO: 2014- move increase into whisper
	numberOfOperations = getNumberOfOperations(ms)*14000;


	total = 0;
	for (i=0;i<numberOfOperations;i++){
		
		if (rIndex1 >= R_ARRAY_SIZE_1 ){
			rIndex1 = 0;
		}
		
		if (rIndex2 >= R_ARRAY_SIZE_2){
			rIndex2 = 0;
		}
		total += rArray1[rIndex1] * rArray2[rIndex2];
		rIndex1++;
		rIndex2++;
//		printf("i %ld\n", i);
	}
	
	if(total==RAND_MAX){
		printf("Just here to make sure total isn't optimized away\n");
	}
	return 0;
}
