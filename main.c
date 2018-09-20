/***************************************************************************
 *
 *   File        : main.c
 *   Student Id  : <758397>
 *   Name        : <JUSTIN JOSEPH BUGEJA>
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include "tasks.h"

#define TIMEDIV 1000.0


int main(int argc, char *argv[]) {
	
	/*Parse Command Line Arguments */
	char* flow_file;
	int resolution;
	FILE *file;

	//Return error if no file or resolution
	if (argc != 3){
		printf("ERROR - enter arguments\n");
		exit(EXIT_FAILURE);
	}
	flow_file = argv[1];
	resolution = atoi(argv[2]);
	//Return error if file cannot be opened
	if ( (file = fopen(flow_file, "r")) == NULL ) {
		printf("ERROR - file could not be opened\n\n");
		exit(EXIT_FAILURE);
	}

	array_t *array = createArr(file);

	fclose(file);

	/* Add timing for each task and output running time in ms */
	 struct timeval start;
	 struct timeval stop;


	/* Task 1: Find the maximum velocity difference */
	gettimeofday(&start, NULL);
	/*Task 1 function*/
	maxveldiff(array);
	/* Elapsed time*/
	gettimeofday(&stop, NULL);
	double elapsed_ms = (stop.tv_sec - start.tv_sec) * TIMEDIV;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / TIMEDIV;
	printf("TASK 1: %.2f ms\n", elapsed_ms);
	
	/* Task 2: Coarser Grid */
	gettimeofday(&start, NULL);
	/*Task 2 function*/
	coarsegrid(array, resolution);
	/* Elapsed time*/
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * TIMEDIV +
			(stop.tv_usec - start.tv_usec) / TIMEDIV;
	printf("TASK 2: %.2f ms\n", elapsed_ms);
	
	/* Task 3: Statistics */
	gettimeofday(&start, NULL);
	/*Task 3 function*/
	velstat(array);
	/* Elapsed time*/
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * TIMEDIV +
			(stop.tv_usec - start.tv_usec) / TIMEDIV;
	printf("TASK 3: %.2f ms\n", elapsed_ms);
	
	/* Task 4: Wake height and visualisation */
	gettimeofday(&start, NULL);
	/*Task 4 function*/
	wakevis(array);
	/* Elapsed time*/
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * TIMEDIV +
			(stop.tv_usec - start.tv_usec) / TIMEDIV;
	printf("TASK 4: %.2f ms\n", elapsed_ms);


	freearray(array);

	return (EXIT_SUCCESS);
}
