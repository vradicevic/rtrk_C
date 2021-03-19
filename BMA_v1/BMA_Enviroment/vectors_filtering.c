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
int filterVectorsFlow(int16_t** vectors, int numOfMatches) {
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

    

    calculateMeans(means, k, ftr_num, items, numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
    findClusters(means, items, numOfMatches, k, ftr_num, belongsTo);

    //cluster_i = calculateVarianceOf(means, vectors, clusterSizes, belongsTo, numOfMatches, k);
    //cluster_i2 = calculateNexBestVariance(means, vectors, clusterSizes, belongsTo, numOfMatches, k, cluster_i);
    ftr_num = 6;
    copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, 2, numOfMatches, 0);
    //copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, cluster_i2, numOfMatches, clusterSizes[cluster_i]);
    //vectors = interVectors;
    numOfMatches = clusterSizes[2];
    vectors = interVectors;
    

    printf("\nMatches after filtering %d", numOfMatches);
    printf("\nCluster sizes: %d; %d; %d; %d;", clusterSizes[0], clusterSizes[1], clusterSizes[2], clusterSizes[3]);
    return numOfMatches;

}