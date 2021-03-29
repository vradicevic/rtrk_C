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

int filterByDistanceFromMean(int16_t** vectors, int16_t** newVectors, float** means, uint8_t* belongsTo, int clusterId, int numOfVectors, int ftr_i,int isAngle) {
    int z=0;
    for (int i = 0; i < numOfVectors; i++) {
        if (belongsTo[i] == clusterId) {
            if ((float)vectors[ftr_i][i] > (0.6 * means[clusterId][ftr_i])) {
                newVectors[0][z] = vectors[0][i];
                newVectors[1][z] = vectors[1][i];
                newVectors[2][z] = vectors[2][i];
                newVectors[3][z] = vectors[3][i];
                newVectors[4][z] = vectors[4][i];
                newVectors[5][z] = vectors[5][i];
                z++;
            }
                
        }
    }
    return z;

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
    uint16_t clusterSizes[10];
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

    for (i = 0; i < 6; i++) {
        interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
    }
    uint8_t angle_ftr_num = 0;
    ftr_num = 1;
    items[0] =vectors[5];
    items[1] =vectors[4];
    //items[2] =vectors[5];
    //items[0] = vectors[5];
    
    

    calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
    findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
    int num = *numOfMatches;
    cluster_i = calculateVarianceOfAngle(means, vectors, clusterSizes, belongsTo, num, k);
    cluster_i2 = calculateNexBestVariance(means, vectors, clusterSizes, belongsTo, num, k, cluster_i);
    ftr_num = 6;
    copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, cluster_i, num, 0);
    
    copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, cluster_i2, num, clusterSizes[cluster_i]);
    
    *numOfMatches = clusterSizes[cluster_i] + clusterSizes[cluster_i2];
    copyVectors(vectors, interVectors, *numOfMatches);

    
    
    //*numOfMatches = clusterSizes[cluster_i];


	items[0] = interVectors[2];
	items[1] = interVectors[3];
	items[2] = interVectors[4];
    items[3] = interVectors[5];
	ftr_num = 4;
	angle_ftr_num = 3;
	k = 3;
	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
    
    
    
    return belongsTo;

}