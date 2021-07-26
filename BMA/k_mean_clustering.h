#ifndef _K_MEAN_CLUSTERING_H_
#define _K_MEAN_CLUSTERING_H_
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "bma_utils.h"




uint8_t calculateVarianceOfAngle(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k);
uint8_t calculateNexBestVariance(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k, uint8_t toIgnore);


void findMinMaxColumns(float* minima, float* maxima, uint8_t ftr_num, int16_t** vectorsFiltered, uint16_t countOfFilteredVectors);
float getRandom(float* minima, float* maxima);
void initializeMeans(float** means, uint8_t k, uint8_t ftr_num, float* minima, float* maxima);
float euclideanDistance(int16_t** vectors, int v_index, float* mean, uint8_t ftr_num);
float minkowskiDistance(int16_t** vectors, int v_index, float* mean, uint8_t ftr_num);
void updateMean(uint16_t clusterSize, float* mean, int16_t** vectors, int v_index, uint8_t ftr_num);
int classify(float** means, int16_t** vectors, int v_index, uint8_t k, uint8_t ftr_num);
void calculateMeans(float** means, uint8_t k, uint8_t ftr_num, int16_t** vectors, uint16_t countOfVectors, int maxIterations, uint8_t* belongsTo, uint16_t* clusterSizes, float* minima, float* maxima,int ftr_angle);
void findClusters(float** means, int16_t** vectors, uint16_t countOfVectors, uint8_t k, uint8_t ftr_num, uint16_t* clusterSizes, uint8_t* clusters);



#endif
