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

int16_t** filterByLength(int16_t** vectors, int16_t** filteredVectors, int* numOfMatches, int ftr_num);

uint8_t* filterVectorsFlowSimple(int16_t** vectors, int* numOfMatches);
float getFtrMeanOfK(int16_t* ftr, uint8_t* belongsTo, int count, int cluster_i);
int getBestKByLengthMean(int16_t* lengths, int k, uint8_t* belongsTo, uint16_t* clusterSizes, int numOfVectors);

uint8_t* filterVectorsFlow(int16_t** vectors, int* numOfMatches);

uint8_t* filterVectorsFlowMoving(int16_t** vectors, int* numOfMatches);

uint8_t* filterVectorsFlowTest(int16_t** vectors, int* numOfMatches);

void copyBelongsTo(uint8_t* src, uint8_t* dest, int count);

void copyClusterSizes(uint16_t* src, uint16_t* dest, int count);

void copyCluster(int16_t** src, int16_t** dest, uint16_t* clusterSizes, uint8_t* belongsTo, uint8_t ftr_num, int cluster_i, int v_s, int interVsOffset);

void copyVectors(int16_t** dest, int16_t** src, int numOfMatches);

int getBestClusterByLength(float** means, int ftrid_length, int k);

float calculateMiddleVarianceAngle(int16_t** vectors, int numOfVectors, uint8_t* belongsTo, uint16_t* clusterSizes, float** means, int k, int ftr_angle);

uint8_t* groupVectors(int16_t** vectors,uint8_t* belongsTo, int numOfVectors, int k_max);

#endif