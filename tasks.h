/***************************************************************************
 *
 *   File        : tasks.h
 *   Student Id  : <758397>
 *   Name        : <JUSTIN JOSEPH BUGEJA>
 *
 ***************************************************************************/

#ifndef TASKS_H

/* structs */
typedef struct stats_t stats_t;
typedef struct listnode_t listnode_t;
typedef struct node_t node_t;
typedef struct flowdata_t flowdata_t;
typedef struct flowdatas_t flowdatas_t;
typedef struct array_t array_t;
typedef struct avgs_t avgs_t;

/* define needed here for buff[] */
#define NVALS 4

/* sorting functions */
int sortU(const void *p, const void *q);
int sortX(const void *p, const void *q);
int sortS(const void *p, const void *q);

/* creating the array */
array_t *createArr(FILE *fr);
void addtoarray(float buff[NVALS], array_t* array, int size);

/* task 2 grid */
void placegrid(listnode_t** grid, flowdata_t* node, int resolution);
flowdatas_t getAvgCell(listnode_t **grid, int x, int y, int resolution);

/* tasks */
void maxveldiff(array_t* array);
void coarsegrid(array_t* array, int resolution);
void velstat(array_t* array);
void wakevis(array_t* array);

/* cleanup */
void freeNodes(listnode_t* grid);
void freearray(array_t *array);

#endif
