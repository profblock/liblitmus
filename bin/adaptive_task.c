/* based_task.c -- A basic real-time task skeleton. 
 *
 * This (by itself useless) task demos how to setup a 
 * single-threaded LITMUS^RT real-time task.
 */

/* First, we include standard headers.
 * Generally speaking, a LITMUS^RT real-time task can perform any
 * system call, etc., but no real-time guarantees can be made if a
 * system call blocks. To be on the safe side, only use I/O for debugging
 * purposes and from non-real-time sections.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Second, we include the LITMUS^RT user space library header.
 * This header, part of liblitmus, provides the user space API of
 * LITMUS^RT.
 */
#include "litmus.h"

/* Next, we define period and execution cost to be constant. 
 * These are only constants for convenience in this example, they can be
 * determined at run time, e.g., from command line parameters.
 *
 * These are in milliseconds.
 */
#define PERIOD            100
#define RELATIVE_DEADLINE 100
#define EXEC_COST         10

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


/* Declare the periodically invoked job. 
 * Returns 1 -> task should exit.
 *         0 -> task should continue.
 */
int job(int count);


int test(){
	int do_exit=0;
	int i;
	struct rt_task param;
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
	
	printf("Service Levels Beginning\n");
	param.service_levels = (struct rt_service_level*)malloc(sizeof(struct rt_service_level)*4);
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

	
	printf("Service Levels Ending\n");

	/*****
	 * 3) Setup real-time parameters. 
	 *    In this example, we create a sporadic task that does not specify a 
	 *    target partition (and thus is intended to run under global scheduling). 
	 *    If this were to execute under a partitioned scheduler, it would be assigned
	 *    to the first partition (since partitioning is performed offline).
	 */
	 printf("init Starting\n");
	CALL( init_litmus() );
	printf("Done init\n");

	/* To specify a partition, do
	 *
	 * param.cpu = CPU;
	 * be_migrate_to(CPU);
	 *
	 * where CPU ranges from 0 to "Number of CPUs" - 1 before calling
	 * set_rt_task_param().
	 */

	CALL( set_rt_task_param(gettid(), &param) );
	printf("Done setting rt_task paramaters");


	/*****
	 * 4) Transition to real-time mode.
	 */
	CALL( task_mode(LITMUS_RT_TASK) );
	printf("Done transitioning\n ");

	/* The task is now executing as a real-time task if the call didn't fail. 
	 */

	i=0;
	printf("%d, %d\n", i, do_exit);
	/*****
	 * 5) Invoke real-time jobs.
	 */ 
	do {
		/* Wait until the next job is released. */
		printf("starting %d\n",i);
		i++;
		sleep_next_period();
		/* Invoke job. */
		do_exit = job(i);		

	} while (!do_exit);

	/*****
	 * 6) Transition to background mode.
	 */

	CALL( task_mode(BACKGROUND_TASK) );
	printf("done\n");
	return 0;
}
/* typically, main() does a couple of things: 
 * 	1) parse command line parameters, etc.
 *	2) Setup work environment.
 *	3) Setup real-time parameters.
 *	4) Transition to real-time mode.
 *	5) Invoke periodic or sporadic jobs.
 *	6) Transition to background mode.
 *	7) Clean up and exit.
 *
 * The following main() function provides the basic skeleton of a single-threaded
 * LITMUS^RT real-time task. In a real program, all the return values should be 
 * checked for errors.
 */
 
int main(int argc, char** argv)
{
	int do_exit=0;
	int i;
	struct rt_task param;
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
	
	param.service_levels = (struct rt_service_level*)malloc(sizeof(struct rt_service_level)*4);
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
	 * 3) Setup real-time parameters. 
	 *    In this example, we create a sporadic task that does not specify a 
	 *    target partition (and thus is intended to run under global scheduling). 
	 *    If this were to execute under a partitioned scheduler, it would be assigned
	 *    to the first partition (since partitioning is performed offline).
	 */
	CALL( init_litmus() );

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
	 * 4) Transition to real-time mode.
	 */
	CALL( task_mode(LITMUS_RT_TASK) );

	/* The task is now executing as a real-time task if the call didn't fail. 
	 */

	i=0;
	/*****
	 * 5) Invoke real-time jobs.
	 */ 
	do {
		/* Wait until the next job is released. */
		i++;
		sleep_next_period();
		/* Invoke job. */
		do_exit = job(i);		

	} while (!do_exit);

	/*****
	 * 6) Transition to background mode.
	 */

	CALL( task_mode(BACKGROUND_TASK) );
	return 0;
}


int job(int count) 
{
	/* Do real-time calculation. */
	printf("Hello\n");
	/* Don't exit. */
	return 0;
	/*
	if (count>=100){
		return 1;
	}
	else 
	{
		return 0;
	}*/
}
