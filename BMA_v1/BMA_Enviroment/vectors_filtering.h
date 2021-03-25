#ifndef _VECTORS_FILTERING_H_
#define _VECTORS_FILTERING_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include "k_mean_clustering.h"

void allocateArgsForKmeans(int16_t ** vectors,int16_t** items, int16_t** interVectors, float** means,uint8_t* belongsTo,float* minima,float* maxima,int k,int ftr_num, int maxMatches);

int16_t** filterByLength(int16_t** vectors, int* numOfMatches, int ftr_num);

uint8_t* filterVectorsFlow(int16_t** vectors, int numOfMatches);

#endif