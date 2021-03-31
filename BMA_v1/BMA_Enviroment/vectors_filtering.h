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

void get2Max(uint16_t* clusterSizes, int* biggestFirst, int* biggestSecond, int* k);

int16_t** filterByLength(int16_t** vectors, int* numOfMatches, int ftr_num);

uint8_t* filterVectorsFlow(int16_t** vectors, int* numOfMatches);

uint8_t* filterVectorsFlowMoving(int16_t** vectors, int* numOfMatches);

uint8_t* filterVectorsFlowTest(int16_t** vectors, int* numOfMatches);

void copyBelongsTo(uint8_t* src, uint8_t* dest, int count);

void copyClusterSizes(uint16_t* src, uint16_t* dest, int count);

void copyCluster(int16_t** src, int16_t** dest, uint16_t* clusterSizes, uint8_t* belongsTo, uint8_t ftr_num, int cluster_i, int v_s, int interVsOffset);

void copyVectors(int16_t** dest, int16_t** src, int numOfMatches);

int getBestClusterByLength(float** means, int ftrid_length, int k);

#endif