#ifndef _K_MEAN_CLUSTERING_H_
#define _K_MEAN_CLUSTERING_H_
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "bma_utils.h"



uint8_t calculateVarianceOf(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k);
uint8_t calculateNexBestVariance(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k, uint8_t toIgnore);
void get2Max(uint16_t* clusterSizes, uint8_t* biggestFirst, uint8_t* biggestSecond, uint8_t* k);
void copyCluster(int16_t** vectors, int16_t** interVectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint8_t ftr_num, uint8_t cluster_i, int v_s, uint8_t interVsOffset);
void findMinMaxColumns(float* minima, float* maxima, uint8_t ftr_num, int16_t** vectorsFiltered, uint16_t countOfFilteredVectors);
float getRandom(float* minima, float* maxima);
void initializeMeans(float** means, uint8_t k, uint8_t ftr_num, float* minima, float* maxima);
float euclideanDistance(int16_t** vectors, int v_index, float* mean, uint8_t ftr_num);
void updateMean(uint16_t clusterSize, float* mean, int16_t** vectors, int v_index, uint8_t ftr_num);
int classify(float** means, int16_t** vectors, int v_index, uint8_t k, uint8_t ftr_num);
void calculateMeans(float** means, uint8_t k, uint8_t ftr_num, int16_t** vectors, uint16_t countOfVectors, int maxIterations, uint8_t* belongsTo, uint16_t* clusterSizes, float* minima, float* maxima);
void findClusters(float** means, int16_t** vectors, uint16_t countOfVectors, uint8_t k, uint8_t ftr_num, uint8_t* clusters);

#endif
