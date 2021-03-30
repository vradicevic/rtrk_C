#ifndef _VECTORS_FILTERING_H_
#define _VECTORS_FILTERING_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include "k_mean_clustering.h"
typedef struct {
	uint8_t* belongsTo;
	int k;
	int ftr_num;
	int ftr_include[6];
	uint16_t* clusterSizes;

}KMeansInfo;

void allocateArgsForKmeans(int16_t ** vectors,int16_t** items, int16_t** interVectors, float** means,uint8_t* belongsTo,float* minima,float* maxima,int k,int ftr_num, int maxMatches);

int16_t** filterByLength(int16_t** vectors, int* numOfMatches, int ftr_num);

uint8_t* filterVectorsFlow(int16_t** vectors, int* numOfMatches);

uint8_t* filterVectorsFlowMoving(int16_t** vectors, int* numOfMatches);

void copyBelongsTo(uint8_t* src, uint8_t* dest, int count);

void copyClusterSizes(uint16_t* src, uint16_t* dest, int count);

#endif