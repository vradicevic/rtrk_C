#include "vectors_filtering.h"


void allocateArgsForKmeans(int16_t** items, int16_t** interVectors, float** means, uint8_t* belongsTo, float* minima, float* maxima,int k, int ftr_num,int maxMatches) {
    means = (float**)malloc(k * sizeof(float));
    int i;
    for (i = 0; i < k; i++) {
        means[i] = (float*)malloc(ftr_num*sizeof(float));
        
    }
    belongsTo = (uint8_t*)malloc(maxMatches*sizeof(uint8_t));
   


    minima = (float*)malloc(ftr_num*sizeof(float));
    

    maxima = (float*)malloc(ftr_num * sizeof(float));
    

    items = (int16_t**)malloc(ftr_num*sizeof(int16_t*)); 
    

    interVectors = (int16_t**)malloc(ftr_num * sizeof(int16_t*));
    
    for (i = 0; i < ftr_num; i++) {
        interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
    }
}
int16_t** filterByLength(int16_t** vectors, int* numOfMatches,int ftr_num) {
	int maxMatches = 14400;
	int16_t** filtered = (int16_t **)malloc(ftr_num*sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		filtered[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}

	
	int index = 0;
	for (int i = 0; i < *numOfMatches; i++) {
		if (vectors[4][i] > 2) {
			for (int j = 0; j < ftr_num; j++) {
				filtered[j][index] = vectors[j][i];
			}
			index++;
		}
	}
	//free(vectors);
	
	*numOfMatches = index;
	return filtered;
}

uint8_t* filterVectorsFlow(int16_t** vectors, int* numOfMatches) {
    int maxMatches = 14400;
    uint8_t cluster_i, cluster_i2;
    int16_t** items;
    int16_t** interVectors;
    float** means;
    uint8_t* belongsTo;
    float* minima;
    float* maxima;
    int k=4;
    int ftr_num=6;
    uint16_t clusterSizes[6];
    means = (float**)malloc(k * sizeof(float));
    int i;
    for (i = 0; i < k; i++) {
        means[i] = (float*)malloc(ftr_num * sizeof(float));

    }
    belongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));



    minima = (float*)malloc(ftr_num * sizeof(float));


    maxima = (float*)malloc(ftr_num * sizeof(float));


    items = (int16_t**)malloc(ftr_num * sizeof(int16_t*));


    interVectors = (int16_t**)malloc(ftr_num * sizeof(int16_t*));

    for (i = 0; i < ftr_num; i++) {
        interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
    }
    uint8_t angle_ftr_num = 0;
    ftr_num = 1;
    items[0] =vectors[5];
    //items[1] =vectors[3];
    //items[2] =vectors[5];
    //items[0] = vectors[5];

    

    calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
    findClusters(means, items, *numOfMatches, k, ftr_num, belongsTo);

    cluster_i = calculateVarianceOf(means, vectors, clusterSizes, belongsTo, *numOfMatches, k);
    //cluster_i2 = calculateNexBestVariance(means, vectors, clusterSizes, belongsTo, numOfMatches, k, cluster_i);
    ftr_num = 6;
    copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, cluster_i, *numOfMatches, 0);
    //copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, cluster_i2, numOfMatches, clusterSizes[cluster_i]);
    *numOfMatches = clusterSizes[cluster_i];


	items[0] = interVectors[2];
	items[1] = interVectors[3];
	items[2] = interVectors[4];
	ftr_num = 3;
	angle_ftr_num = ftr_num + 1;
	k = 2;
	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, belongsTo);

    vectors = interVectors;
    return belongsTo;

}