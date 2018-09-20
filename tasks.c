/***************************************************************************
 *
 *   File        : tasks.c
 *   Student Id  : <758397>
 *   Name        : <JUSTIN JOSEPH BUGEJA>
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include "tasks.h"

/* CONSTANTS */
#define OUTPUTFILE1 "task1.csv"
#define OUTPUTFILE2 "task2.csv"
#define OUTPUTFILE3 "task3.csv"
#define OUTPUTFILE4 "task4_1.csv"
#define DELIMINATOR ","
#define FLOATVAL "%.6f"
#define FLT_MIN 0.0f
#define FLT_MAX 100.0f
#define MAX_X 70
#define MIN_X 10
#define MAX_Y 20
#define MIN_Y -20
#define MINX (double)20.0
#define XCOL 0
#define YCOL 1
#define UCOL 2
#define VCOL 3
#define SIZE 1
#define BUFF 100
#define EPSILON 0.000001
#define EPSILON1 (double)0.050
#define THRESHINCR (double)0.10
#define THRESHSTART (double)0.50
#define WAKESTART 10
#define WAKEINCR 5

/* struct for the raw data*/
struct flowdata_t {
	float x, y, u, v;
};

/* struct for avg'd values*/
struct flowdatas_t {
	float x, y, u, v, s;
};

/* struct to store in a linked list */
struct listnode_t {
	flowdata_t *flowval;
	float s;
	listnode_t *next;
};

/*array_t holds an array of flow data structs*/
struct array_t {
	flowdata_t* flowval;
	int size;
	int mem;
};

/* struct for the stats of task 3 */
struct stats_t {
	double thresh;
	int points;
	float percentage;
};

/* Task 1 */
void maxveldiff(array_t* array) {

//MAX AND MIN FINDING

	flowdata_t maxuf;
	maxuf.u = FLT_MIN;
	flowdata_t minuf;
	minuf.u = FLT_MAX;
	flowdata_t maxvf;
	maxvf.v = FLT_MIN;
	flowdata_t minvf;
	minvf.v = FLT_MAX;

	/* iterate through array to find the max and mins */
	for (int i = 0; i < array->size; i++) {
		/* checking whether x is > MIN value */
		float hx = array->flowval[i].x;
		if (hx > MINX) {
			/* current values */
			float hy = array->flowval[i].y;
			float hu = array->flowval[i].u;
			float hv = array->flowval[i].v;
			/* compare to max and mins */
			if (hu > maxuf.u) {
				maxuf.x = hx;
				maxuf.y = hy;
				maxuf.u = hu;
				maxuf.v = hv;
			}
			if (hu < minuf.u) {
				minuf.x = hx;
				minuf.y = hy;
				minuf.u = hu;
				minuf.v = hv;
			}
			if (hv > maxvf.v) {
				maxvf.x = hx;
				maxvf.y = hy;
				maxvf.u = hu;
				maxvf.v = hv;
			}
			if (hv < minvf.v) {
				minvf.x = hx;
				minvf.y = hy;
				minvf.u = hu;
				minvf.v = hv;
			}
		}
	}

	/* PRINT OUTPUT TO FILE*/
	FILE *fp = fopen(OUTPUTFILE1, "w");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(EXIT_FAILURE);
	}

	/* printing */
	fprintf(fp, "x,y,u,v\n");
	fprintf(fp, FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL"\n", maxuf.x, maxuf.y,
			maxuf.u, maxuf.v);
	fprintf(fp, FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL"\n", minuf.x, minuf.y,
			minuf.u, minuf.v);
	fprintf(fp, FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL"\n", maxvf.x, maxvf.y,
			maxvf.u, maxvf.v);
	fprintf(fp, FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL"\n", minvf.x, minvf.y,
			minvf.u, minvf.v);
	fclose(fp);

}

/* task 2 */
void coarsegrid(array_t* array, int resolution) {

	/* checking if resolution is positive */
	if (resolution < 1) {
		printf("Resolution must be positive!\n");
		exit(EXIT_FAILURE);
	}

	/* creating the grid */
	listnode_t** grid = malloc(sizeof(listnode_t*) * resolution * resolution);
	if (!grid) {
		perror("malloc error\n");
		exit(EXIT_FAILURE);
	}
	/* initialising the grid */
	for (int k = 0; k < resolution; k++) {
		for (int j = 0; j < resolution; j++) {
			grid[k * resolution + j] = NULL;
		}
	}

	/* the length of each cell in the grid */
	float xceil = 1.0 * (MAX_X - MIN_X) / (1.0 * resolution);
	float yceil = 1.0 * (MAX_Y - MIN_Y) / (1.0 * resolution);

	/* iterate through array to put each line in their corresponding cells */
	for (int i = 0; i < array->size; i++) {
		/* finding the cells for each value */
		float xcell = (array->flowval[i].x - MIN_X) / xceil + 0.5;
		float ycell = (array->flowval[i].y - MIN_Y) / yceil + 0.5;
		/* cell bordering can only happen on non-truncated integer values */
		int xcell1 = (array->flowval[i].x - MIN_X) / xceil + 0.5;
		int ycell1 = (array->flowval[i].y - MIN_Y) / yceil + 0.5;
		int xcell2 = -1;
		int ycell2 = -1;
		//To account for x = max_x
		if (xcell1 == resolution) {
			xcell1--;
		}
		//To account for y = max_y
		if (ycell1 == resolution) {
			ycell1--;
		}
		/* check if xcell is on border */
		if (xcell1 != 0) {
			if (fabs(xcell - xcell1) < EPSILON) {
				xcell2 = xcell1 - 1;
			}
		}
		/* check if ycell is on border */
		if (ycell1 != 0) {
			if (fabs(ycell - ycell1) < EPSILON) {
				ycell2 = ycell1 - 1;
			}
		}

		/* placing each value in their cells */
		placegrid(&grid[xcell1 * resolution + ycell1], &array->flowval[i],
				resolution);
		/* place xcell on bordered cell */
		if (xcell2 != -1) {
			placegrid(&grid[xcell2 * resolution + ycell1], &array->flowval[i],
					resolution);
		}
		/* place ycell on bordered cell */
		if (ycell2 != -1) {
			placegrid(&grid[xcell1 * resolution + ycell2], &array->flowval[i],
					resolution);
		}
		/* place xcell and ycell on bordered cell */
		if (ycell2 != -1 && xcell2 != -1) {
			placegrid(&grid[xcell2 * resolution + ycell2], &array->flowval[i],
					resolution);
		}

	}

	/* initialise avgS array */
	flowdatas_t avg[resolution * resolution];
	/* iterate through each cell */
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			if (grid[x * resolution + y] != NULL) {
				/* find the avg value for each cell */
				avg[x * resolution + y] = getAvgCell(&grid[x * resolution + y],
						x, y, resolution);
			}
		}
	}

	/* sort the data with respect to S values */
	qsort(avg, resolution * resolution, sizeof(flowdatas_t), sortS);

	/* PRINT OUTPUT TO FILE*/
	FILE *fp = fopen(OUTPUTFILE2, "w");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(EXIT_FAILURE);
	}

	/* print header */
	fprintf(fp, "x,y,u,v,S\n");
	/* iteratively print each avg and s score */
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			/* incase a cell is not occupied */
			if (grid[i * resolution + j] != NULL) {
				fprintf(fp,
				FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL","FLOATVAL"\n",
						avg[i * resolution + j].x, avg[i * resolution + j].y,
						avg[i * resolution + j].u, avg[i * resolution + j].v,
						avg[i * resolution + j].s);
			}
		}
	}
	fclose(fp);
	/* iteratively free each node */
	for (int f = 0; f < resolution * resolution; f++) {
		freeNodes(grid[f]);
	}
	/* free the allocated grid */
	free(grid);
}

/* get the avg of the cell */
flowdatas_t getAvgCell(listnode_t **grid, int x, int y, int resolution) {
	listnode_t *ptr = *grid;
	float avgS = 0, sumx = 0, sumy = 0, sumu = 0, sumv = 0;
	int count = 0;
	/* iterate through the linked list while summing the values*/
	while (ptr) {
		sumx += ptr->flowval->x;
		sumy += ptr->flowval->y;
		sumu += ptr->flowval->u;
		sumv += ptr->flowval->v;
		count++;
		ptr = ptr->next;
	}
	/* s score formula */
	avgS = 100
			* ((sqrt(
					pow(sumu / (double) count, 2)
							+ pow(sumv / (double) count, 2)))
					/ (sqrt(
							pow(sumx / (double) count, 2)
									+ pow(sumy / (double) count, 2))));
	flowdatas_t avg;
	/* compute avgs and store in struct */
	avg.s = avgS;
	avg.u = sumu / (double) count;
	avg.v = sumv / (double) count;
	avg.x = sumx / (double) count;
	avg.y = sumy / (double) count;
	/* return avgS */
	return avg;
}

/* Task 3 */
void velstat(array_t* array) {

	/* Sort array with respect to u */
	qsort(array->flowval, array->size, sizeof(flowdata_t), sortU);

	/* initialise variables */
	float maxuval = array->flowval[array->size - 1].u;
	int size = (maxuval + THRESHINCR ) / THRESHINCR;
	stats_t threshstat[size];

	double thresh;
	int level = 0;
	int i = 0;

	/* count through the thresh values */
	for (thresh = THRESHSTART ; thresh < maxuval + THRESHINCR ; thresh +=
	THRESHINCR) {
		/* iterate through each u value until it is greater than or equal to
		 * its thresh */
		for (; fabs(array->flowval[i].u) < thresh && i < array->size - 1; i++) {
		}
		/* last element in array is the size of the array -1 */
		if (i == array->size - 1) {
			i++;
		}
		/* store the stats */
		threshstat[level].percentage = (i / (1.0 * array->size)) * 100;
		threshstat[level].points = i;
		threshstat[level].thresh = thresh;
		level++;

	}

	/* PRINT OUTPUT TO FILE*/
	FILE *fp = fopen(OUTPUTFILE3, "w");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(EXIT_FAILURE);
	}
	/* print header */
	fprintf(fp, "threshold,points,percentage\n");
	/* iterate through each thresh level and print their values */
	for (int j = 0; j < level; j++) {
		fprintf(fp, FLOATVAL",%d,"FLOATVAL"\n", threshstat[j].thresh,
				threshstat[j].points, threshstat[j].percentage);
	}
	fclose(fp);
}

/* Task 4 */
void wakevis(array_t* array) {
	/* initialise variables */
	int w, k = 0, pos = 0, arrnum = (MAX_X - MIN_X) / WAKEINCR;
	int maxpos[arrnum];
	float maxu = 0, yval;

	/* sort array with repsect to x */
	qsort(array->flowval, array->size, sizeof(flowdata_t), sortX);

	/* go through each x wake value*/
	for (w = MIN_X; w < MAX_X; w += WAKEINCR) {
		/* initialise for each x wake value */
		maxu = 0;
		yval = FLT_MAX;
		maxpos[k] = 0;
		/* iterate through each x value */
		for (; pos < array->size; pos++) {
			/* stop when x value is greater than x wake value */
			if (array->flowval[pos].x > (w + EPSILON1 )) {
				break;
			}
			/* if the x value is within 0.05 of the x wake value */
			if (fabs(array->flowval[pos].x - 1.0 * w) < EPSILON1) {
				/* if u is greater than current max recorded */
				if (array->flowval[pos].u > maxu) {
					maxu = array->flowval[pos].u;
					maxpos[k] = pos;
					yval = array->flowval[pos].y;

					/* I found that using the minimum fabs(yval) gives a better
					 * curve than the plot shown in the Assingment 1 Notes document*/

					/* if u is the same as current max recorded but y is lesser */
				} else if (fabs(array->flowval[pos].u - maxu) < EPSILON
						&& array->flowval[pos].y < yval) {
					maxpos[k] = pos;
					yval = array->flowval[pos].y;

				}
			}
		}
		/* move to next wake value */
		k++;
	}

	/* PRINT OUTPUT TO FILE - choose min y for each u*/
	FILE *fp = fopen(OUTPUTFILE4, "w");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(EXIT_FAILURE);
	}

	int posmax;
	/* print header */
	fprintf(fp, "x,y_h\n");
	/* iteratitvely print each wake value */
	for (int printcount = 0; printcount < k; printcount++) {
		posmax = maxpos[printcount];
		if (maxpos[printcount] != 0) {
			fprintf(fp, FLOATVAL","FLOATVAL"\n", array->flowval[posmax].x,
					fabs(array->flowval[posmax].y));
		}
	}
	fclose(fp);

	int i, j, z;
	int n = 12; // Location in x for wake visualization
	float* yheight;
	yheight = (float*) calloc(n, sizeof(float));
	/* Task 4: Part 2, nothing is to be changed here
	 Remember to output the spacing into the array yheight
	 for this to work. You also need to initialize i,j and
	 yheight so the skeleton as it stands will not compile */
	for (int m = 0; m < n; m++) {
		z = maxpos[m];
		yheight[m] = ceil(10 * fabs(array->flowval[z].y));
	}

	FILE *ft42;
	ft42 = fopen("task4_2.txt", "w");
	for (j = 11; j >= 0; j--) {
		for (i = 0; i < yheight[j] - yheight[0] + 4; i++) {
			fprintf(ft42, " ");
		}
		fprintf(ft42, "*\n");
	}
	for (i = 0; i < 5; i++) {
		fprintf(ft42, "III\n");
	}
	for (j = 0; j < 12; j++) {
		for (i = 0; i < yheight[j] - yheight[0] + 4; i++) {
			fprintf(ft42, " ");
		}
		fprintf(ft42, "*\n");
	}
	fclose(ft42);

	/* Cleanup */
	free(yheight);
}

/* sort with respect to s in descending order */
int sortS(const void *p, const void *q) {
	/* cast to type */
	float l = ((flowdatas_t *) p)->s;
	float r = ((flowdatas_t *) q)->s;
	/* sorting equals, greater than, or less than respectively */
	if (fabs(l - r) < EPSILON) {
		return 0;
	} else if (l > r) {
		return -1;
	}
	return 1;
}

/* sort with respect to magnitude of u in ascending order */
int sortU(const void *p, const void *q) {
	/* cast to type */
	float l = fabs(((flowdatas_t *) p)->u);
	float r = fabs(((flowdatas_t *) q)->u);
	/* sorting equals, greater than, or less than respectively */
	if (fabs(l - r) < EPSILON) {
		return 0;
	} else if (l > r) {
		return 1;
	}
	return -1;
}

/* sort with respect to x in ascending order */
int sortX(const void *p, const void *q) {
	/* cast to type */
	float l = ((flowdatas_t *) p)->x;
	float r = ((flowdatas_t *) q)->x;
	/* sorting equals, greater than, or less than respectively */
	if (fabs(l - r) < EPSILON) {
		return 0;
	} else if (l > r) {
		return 1;
	}
	return -1;
}

/* placing each value in their cells */
void placegrid(listnode_t** grid, flowdata_t* node, int resolution) {

	/* allocating space for the values */
	listnode_t *new = malloc(sizeof(listnode_t));
	if (!new) {
		perror("malloc error\n");
		exit(EXIT_FAILURE);
	}
	/* initialising */
	new->flowval = node;
	new->next = NULL;
	/* place at start if not filled */
	if (*grid == NULL) {
		*grid = new;
	} else {
		/* else iterate through linked list until spot is found*/
		listnode_t *ptr = *grid;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new;
	}

}

/* read in values from data and create an array */
array_t *createArr(FILE *fr) {
	/* mallocing space */
	array_t *array = malloc(sizeof(array_t));
	array->flowval = malloc(sizeof(flowdata_t) * SIZE);
	array->mem = SIZE;
	if (array == NULL || array->flowval == NULL) {
		perror("malloc array\n");
		exit(EXIT_FAILURE);
	}
	/* initialising buffers */
	char buff[BUFF];
	float vals[NVALS];
	int count = -1;
	/* read each line */
	while (fgets(buff, BUFF, fr) != NULL) {
		/* skip header line */
		if (count == -1) {
			count++;
			continue;
		}
		count++;
		char *split;
		/* split the line into 4 values */
		split = strtok(buff, ",");
		for (int i = 0; i < NVALS; i++) {
			vals[i] = atof(split);
			split = strtok(NULL, ",");
		}
		/* add the values into the array */
		addtoarray(vals, array, count);
	}
	/* return filled array */
	return array;
}

/* adding each value into array */
void addtoarray(float buff[NVALS], array_t* array, int size) {
	if (array == NULL) {
		printf("array STRUCT NOT FOUND");
		exit(EXIT_FAILURE);
	}
	/* reallocs size if not big enough */
	if (size > array->mem) {
		flowdata_t* temp = realloc(array->flowval,
				sizeof(flowdata_t) * size * 2);
		if (!temp) {
			perror("realloc Node");
			exit(EXIT_FAILURE);
		}
		array->flowval = temp;
		array->mem = size * 2;
	}
	/* copy data into array from buffer */
	array->size = size;
	array->flowval[size - 1].x = buff[XCOL];
	array->flowval[size - 1].y = buff[YCOL];
	array->flowval[size - 1].u = buff[UCOL];
	array->flowval[size - 1].v = buff[VCOL];
}

/* recursively free nodes from the grid */
void freeNodes(listnode_t* grid) {
	/* if grid contains a value */
	if (grid) {
		if (grid->next) {
			/* traverse through linked list */
			freeNodes(grid->next);
		}
		free(grid);
	}
}

/* free the array and data stored */
void freearray(array_t *array) {
	free(array->flowval);
	free(array);
}
