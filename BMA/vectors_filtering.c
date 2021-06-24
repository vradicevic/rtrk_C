#include "vectors_filtering.h"

void copyCluster(int16_t** src, int16_t** dest, uint16_t* clusterSizes, uint8_t* belongsTo, uint8_t ftr_num, int cluster_i, int v_s, uint8_t interVsOffset) {
	int i, j, z = 0;
	for (i = 0; i < v_s; i++) {
		if (cluster_i == belongsTo[i]) {
			for (j = 0; j < ftr_num; j++) {
				dest[j][interVsOffset + z] = src[j][i];
			}
			//printf("\nCopy clusters angle: %d\n", interVectors[5][interVsOffset]);
			z++;
		}
	}
	//printf("\nValue of z %d a cluster sizes %d\n", z,clusterSizes[cluster_i]);
}
void copyVectors(int16_t** dest, int16_t** src, int numOfMatches) {
	for (int i = 0; i < numOfMatches; i++) {
		dest[0][i] = src[0][i];
		dest[1][i] = src[1][i];
		dest[2][i] = src[2][i];
		dest[3][i] = src[3][i];
		dest[4][i] = src[4][i];
		dest[5][i] = src[5][i];
		//printf("\nCopy vektors angle: %d\n", vectors[5][i]);
	}
}
void copyBelongsTo(uint8_t* src, uint8_t* dest, int count) {
	int i;
	for (i = 0; i < count; i++) {
		dest[i] = src[i];
	}
}
void copyClusterSizes(uint8_t* src, uint8_t* dest, int count) {
	int i;
	for (i = 0; i < count; i++) {
		dest[i] = src[i];
	}
}


int16_t** filterByLength(int16_t** vectors, int16_t** filteredVectors, int* numOfMatches,int ftr_num) {
	int maxMatches = 14400;
	

	
	int index = 0;
	for (int i = 0; i < *numOfMatches; i++) {
		if (vectors[4][i] > 2 && vectors[4][i]< 50) {
			for (int j = 0; j < ftr_num; j++) {
				filteredVectors[j][index] = vectors[j][i];
			}
			index++;
		}
	}
	
	
	*numOfMatches = index;
	return filteredVectors;
}

float calculateVariance(int16_t* items, float mean, int items_num, int isAngle) {
	float var = 0;
	for (int i = 0; i < items_num; i++) {
		if (isAngle) {
			var += CAST_ANGLE_DIS((abs((int16_t)mean - items[i])) % 360);
		}
		else var += fabs(mean - (float)items[i]);
	}
	
	var = var / items_num;
	return var;

}
void get2Max(uint16_t* clusterSizes, int* biggestFirst, int* biggestSecond, int* k) {
	volatile int i;
	(*biggestFirst) = 0;
	(*biggestSecond) = 1;
	for (i = 0; i < *k; i++) {
		if (clusterSizes[i] > clusterSizes[*biggestFirst]) {
			*biggestFirst = i;
		}
	}
	for (i = 0; i < *k; i++) {
		if (*biggestFirst != i) {
			if (clusterSizes[i] > clusterSizes[*biggestSecond]) {
				*biggestSecond = i;
			}
		}
	}
	
}
int getBestClusterByLength(float** means, int ftrid_length, int k) {
	int best = 0;
	for (int j = 0; j < k; j++) {
		if (means[j][ftrid_length] > means[best][ftrid_length]) {
			best = j;
		}
	}
	
	return best;
}
int getBiggestCluster(uint16_t* clusterSizes, int k) {
	int best = 0;

	for (int i = 0; i < k; i++) {
		if (clusterSizes[i] > clusterSizes[best]) {
			best = i;
		}
	}

	return best;
}
uint8_t* filterVectorsFlowSimple(int16_t** vectors, int* numOfMatches) {
	int maxMatches = 14400;
	int cluster_i, cluster_i2;
	int16_t** items;
	int16_t** cumulativeVectors;
	int16_t** interVectors;
	float** means;
	uint8_t* belongsTo;
	uint8_t* tempBelongsTo;
	float* minima;
	float* maxima;
	uint8_t k = 16;
	int ftr_num = 6;
	uint16_t clusterSizes[16];
	uint16_t tempClusterSizes[16];
	means = (float**)malloc(k * sizeof(float));
	int i;
	for (i = 0; i < k; i++) {
		means[i] = (float*)malloc(ftr_num * sizeof(float));

	}

	belongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));
	tempBelongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));


	minima = (float*)malloc(ftr_num * sizeof(float));


	maxima = (float*)malloc(ftr_num * sizeof(float));


	items = (int16_t**)malloc(ftr_num * sizeof(int16_t*));


	interVectors = (int16_t**)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < 6; i++) {
		interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	cumulativeVectors = (int16_t**)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < ftr_num; i++) {
		cumulativeVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	k = 12;
	uint8_t angle_ftr_num = 0;
	ftr_num = 2;
	items[0] = vectors[2];
	items[1] = vectors[3];
	items[2] = vectors[2];
	items[3] = vectors[3];




	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
	int temp, k_temp, best, cummulativeSize = 0;
	//cluster_i = getBestKByLengthMean(vectors[4], k, belongsTo, clusterSizes, numOfMatches);
	for (int i = 0; i < k; i++) {
		temp = *numOfMatches;
		ftr_num = 6;
		copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, i, temp, 0);
		temp = clusterSizes[i];
		k_temp = 2;
		ftr_num = 1;
		angle_ftr_num = 0;
		items[0] = interVectors[5];
		items[1] = interVectors[5];
		calculateMeans(means, k_temp, ftr_num, items, temp, 10000, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
		findClusters(means, items, temp, k_temp, ftr_num, tempClusterSizes, tempBelongsTo);
		int16_t* lengths = interVectors[4];
		
		cluster_i = getBestKByLengthMean(interVectors[4], k_temp, tempBelongsTo, tempClusterSizes, temp);
		cummulativeSize++;
	}
	*numOfMatches = cummulativeSize;
	copyVectors(vectors, cumulativeVectors, cummulativeSize);

	return belongsTo;
}
int getBestKByLengthMean(int16_t* lengths, int k,uint8_t* belongsTo,uint16_t*clusterSizes,int numOfVectors) {
	int winner = 0;
	float bestMean = 9999999.999;
	float mean;
	int sum = 0;
	//printf("\nNum of Vs. %d, vrijednost na %d\n",numOfVectors,lengths[numOfVectors-1]);
	for (int j = 0; j < k; j++) {
		sum = 0;
		
		for (int i = 0; i < numOfVectors; i++) {
			if (j == belongsTo[i]) {
				sum += (int)lengths[i];
			}
			
		}
		mean = (sum / (float)clusterSizes[j]);
		if (bestMean > mean){
			bestMean = mean;
			winner = j;
		}

	}
	return winner;
}
float getFtrMeanOfK(int16_t* ftr,uint8_t* belongsTo, int count,int cluster_i) {
	int sum=0;
	float result = 0;
	int size = 0;
	for (int i = 0; i < count; i++) {
		if (cluster_i == belongsTo[i]) {
			sum += ftr[i];
			size++;
		}
		
	}
	result = sum / (float)size;
	return result;
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

    for (i = 0; i < ftr_num; i++) {
        interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
    }

	
	
    uint8_t angle_ftr_num = 0;
    ftr_num = 1;
    items[0] =vectors[5];
    items[1] =vectors[3];
    items[2] =vectors[4];
    items[3] = vectors[5];
    
    

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


	items[0] = vectors[2];
	items[1] = vectors[3];
	items[2] = vectors[4];
    items[3] = vectors[5];
	ftr_num = 2;
	angle_ftr_num = 3;
	k = 3;
	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
	uint8_t first, second;
	get2Max(clusterSizes, &first, &second, &k);
	ftr_num = 6;
	copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, first, num, 0);
	copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, second, num, clusterSizes[first]);
	*numOfMatches = clusterSizes[first] + clusterSizes[second];
	copyVectors(vectors, interVectors, *numOfMatches);
	items[0] = vectors[2];
	items[1] = vectors[3];
	items[2] = vectors[5];
	ftr_num = 3;
	angle_ftr_num = 2;
	k = 2;
	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);


    return belongsTo;

}


uint8_t* filterVectorsFlowMoving(int16_t** vectors, int* numOfMatches) {
    
    int maxMatches = 14400;
    int cluster_i, cluster_i2;
    int16_t** items;
	int16_t** cumulativeVectors;
    int16_t** interVectors;
    float** means;
    uint8_t* belongsTo;
	uint8_t* tempBelongsTo;
    float* minima;
    float* maxima;
    uint8_t k=10;
    int ftr_num=6;
    uint16_t clusterSizes[16];
	uint16_t tempClusterSizes[16];
    means = (float**)malloc(k * sizeof(float*));
    int i;
    for (i = 0; i < k; i++) {
        means[i] = (float*)malloc(ftr_num * sizeof(float));

    }

    belongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));
	tempBelongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));


    minima = (float*)malloc(ftr_num * sizeof(float));


    maxima = (float*)malloc(ftr_num * sizeof(float));


    items = (int16_t**)malloc(ftr_num * sizeof(int16_t*));


    interVectors = (int16_t**)malloc(ftr_num * sizeof(int16_t*));

    for (i = 0; i < 6; i++) {
        interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
    }
	cumulativeVectors = (int16_t **)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < ftr_num; i++) {
		cumulativeVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}


	k = 10;
    uint8_t angle_ftr_num = 0;
    ftr_num = 1;
    items[0] =vectors[5];
    
    
    

    calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
    findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
	copyBelongsTo(belongsTo, tempBelongsTo, *numOfMatches);
	copyClusterSizes(clusterSizes, tempClusterSizes, k);
	//printf("\nCluster sizes: %d; %d; %d; %d;\n", clusterSizes[0], clusterSizes[1], clusterSizes[2], clusterSizes[3]);
	ftr_num = 6;
	
	int temp,k_temp= k,best,cummulativeSize=0;
	//get2Max(clusterSizes, &cluster_i, &cluster_i2, &k);
	for (int i = 0; i < k; i++) {
		temp = *numOfMatches;
		ftr_num = 6;
		copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, i, temp, 0);
		temp = clusterSizes[i];
		k_temp = 2;
		ftr_num = 1;
		angle_ftr_num = 0;
		items[0] = interVectors[5];
		
		calculateMeans(means, k_temp, ftr_num, items, temp, 10000, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
		findClusters(means, items, temp, k_temp, ftr_num, tempClusterSizes, tempBelongsTo);
		best = getBiggestCluster(tempClusterSizes, k_temp);
		copyCluster(interVectors, interVectors, tempClusterSizes, tempBelongsTo, 6, best, temp, 0);

		temp = tempClusterSizes[best];
		items[0] = interVectors[4];
		angle_ftr_num = 1;
		calculateMeans(means, k_temp, ftr_num, items, temp, 10000, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
		findClusters(means, items, temp, k_temp, ftr_num, tempClusterSizes, tempBelongsTo);
		best = getBestClusterByLength(means, 0, k_temp);
		copyCluster(interVectors, cumulativeVectors, tempClusterSizes, tempBelongsTo, 6, best, temp, cummulativeSize);
		
		cummulativeSize += tempClusterSizes[best];
		//printf("\nVarijanca klaster: %f\n", calculateVariance(interVectors[5], means[best][5], tempClusterSizes[best], 1));

	}
	*numOfMatches = cummulativeSize;
	copyVectors(vectors, cumulativeVectors, cummulativeSize);
	items[0] = vectors[5];
	
	ftr_num = 1;
	angle_ftr_num = 0;
	k = 4;

	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);


    return belongsTo;

}


uint8_t* filterVectorsBackSub(int16_t** vectors, int* numOfMatches) {

	int maxMatches = 14400;
	int cluster_i, cluster_i2;
	int16_t** items;
	int16_t** cumulativeVectors;
	int16_t** interVectors;
	float** means;
	uint8_t* belongsTo;
	uint8_t* tempBelongsTo;
	float* minima;
	float* maxima;
	uint8_t k = 10;
	int ftr_num = 6;
	uint16_t clusterSizes[16];
	uint16_t tempClusterSizes[16];
	int k_best = 1;
	float middleVariance_best = 9999;
	float middleVariance = 0;



	means = (float**)malloc(k * sizeof(float*));
	int i;
	for (i = 0; i < k; i++) {
		means[i] = (float*)malloc(ftr_num * sizeof(float));

	}

	belongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));
	tempBelongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));


	minima = (float*)malloc(ftr_num * sizeof(float));


	maxima = (float*)malloc(ftr_num * sizeof(float));


	items = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));


	interVectors = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < 6; i++) {
		interVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	cumulativeVectors = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < ftr_num; i++) {
		cumulativeVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}


	k = 10;
	uint8_t angle_ftr_num = 0;
	ftr_num = 1;
	items[0] = vectors[5];
	angle_ftr_num = 5;
	ftr_num = 6;
	items[0] = vectors[0];
	items[1] = vectors[1];
	items[2] = vectors[2];
	items[3] = vectors[3];
	items[4] = vectors[4];
	items[5] = vectors[5];
	int k_max = 10;
	for (k = 1; k <= k_max; k++) {
		calculateMeans(means, k, ftr_num/*ftr_num*/, items, *numOfMatches, 1000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
		findClusters(means, items, *numOfMatches, k, ftr_num/*ftr_num*/, clusterSizes, belongsTo);
		middleVariance = calculateMiddleVarianceAngle(vectors, *numOfMatches, belongsTo, clusterSizes, means, k, *numOfMatches);
		if (middleVariance < middleVariance_best) {
			middleVariance_best = middleVariance;
			k_best = k;
		}
	}
	k = k_best;
	printf("\nBest k = %d", k);
	calculateMeans(means, k, ftr_num, items, *numOfMatches, 1000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
	copyBelongsTo(belongsTo, tempBelongsTo, *numOfMatches);
	copyClusterSizes(clusterSizes, tempClusterSizes, k);
	//printf("\nCluster sizes: %d; %d; %d; %d;\n", clusterSizes[0], clusterSizes[1], clusterSizes[2], clusterSizes[3]);
	ftr_num = 6;

	int temp, k_temp = k, best, cummulativeSize = 0;
	//get2Max(clusterSizes, &cluster_i, &cluster_i2, &k);
	for (int i = 0; i < k; i++) {
		temp = *numOfMatches;
		ftr_num = 6;
		copyCluster(vectors, interVectors, clusterSizes, belongsTo, ftr_num, i, temp, 0);
		temp = clusterSizes[i];
		k_temp = 2;
		ftr_num = 1;
		angle_ftr_num = 3;
		items[0] = interVectors[4];

		calculateMeans(means, k_temp, ftr_num, items, temp, 10000, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
		findClusters(means, items, temp, k_temp, ftr_num, tempClusterSizes, tempBelongsTo);
		best = getBiggestCluster(tempClusterSizes, k_temp);
		//copyCluster(interVectors, interVectors, tempClusterSizes, tempBelongsTo, 6, best, temp, 0);
		
		//temp = tempClusterSizes[best];
		//items[0] = interVectors[4];
		//angle_ftr_num = 1;
		//calculateMeans(means, k_temp, ftr_num, items, temp, 10000, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
		//findClusters(means, items, temp, k_temp, ftr_num, tempClusterSizes, tempBelongsTo);
		//best = getBestClusterByLength(means, 0, k_temp);
		copyCluster(interVectors, cumulativeVectors, tempClusterSizes, tempBelongsTo, 6, best, temp, cummulativeSize);

		cummulativeSize += tempClusterSizes[best];
		//printf("\nVarijanca klaster: %f\n", calculateVariance(interVectors[5], means[best][5], tempClusterSizes[best], 1));

	}
	*numOfMatches = cummulativeSize;
	copyVectors(vectors, cumulativeVectors, cummulativeSize);
	items[0] = vectors[5];

	ftr_num = 1;
	angle_ftr_num = 0;
	k = k_best;

	calculateMeans(means, k, ftr_num, items, *numOfMatches, 10000, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);


	return belongsTo;

}


float calculateMiddleVarianceAngle(int16_t** vectors, int numOfVectors, uint8_t* belongsTo, uint16_t* clusterSizes, float** means, int k,int ftr_angle) {
	int variance[10];
	float sumVariance=0;
	for (int i = 0; i < k; i++) {
		variance[i] = 0;
	}
	int16_t a;
	for (int i = 0; i < numOfVectors; i++) {
		variance[belongsTo[i]] +=pow((CAST_ANGLE_DIS(abs(((int16_t)means[belongsTo[i]][ftr_angle] - vectors[5][i])) % 360)), 2);
	}
	for (int i = 0; i < k; i++) {
		sumVariance+= (variance[i] / (float)clusterSizes[i]);
	}
	sumVariance = sumVariance / k;
	return sumVariance;

}

uint8_t* groupVectors(int16_t** vectors, uint8_t* belongsTo, int numOfVectors, int k_max) {
	float** means;
	int16_t** items;

	float* minima, *maxima;
	uint16_t clusterSizes[10];

	int k_best = 1;
	float middleVariance_best = 9999;
	float middleVariance = 0;
	int ftr_num = 6;
	int k;
	means = (float**)malloc(k_max * sizeof(float*));
	int i;
	for (i = 0; i < k_max; i++) {
		means[i] = (float*)malloc(ftr_num * sizeof(float));

	}

	
	minima = (float*)malloc(ftr_num * sizeof(float));


	maxima = (float*)malloc(ftr_num * sizeof(float));


	items = (int16_t**)malloc(ftr_num * sizeof(int16_t*));

	int ftr_angle = 5;
	ftr_num = 6;
	items[0] = vectors[0];
	items[1] = vectors[1];
	items[2] = vectors[2];
	items[3] = vectors[3];
	items[4] = vectors[5];
	items[5] = vectors[5];
	
	for (k = 1; k <= k_max; k++) {
		calculateMeans(means, k, ftr_num/*ftr_num*/, items, numOfVectors, 100, belongsTo, clusterSizes, minima, maxima, ftr_angle);
		findClusters(means, items, numOfVectors, k, ftr_num/*ftr_num*/, clusterSizes, belongsTo);
		middleVariance = calculateMiddleVarianceAngle(vectors, numOfVectors, belongsTo, clusterSizes, means, k, ftr_angle);
		if (middleVariance < middleVariance_best) {
			middleVariance_best = middleVariance;
			k_best = k;
		}
	}
	k = k_best;
	printf("\nBest k = %d", k);
	calculateMeans(means, k, ftr_num, items, numOfVectors, 100, belongsTo, clusterSizes, minima, maxima, ftr_angle);
	findClusters(means, items, numOfVectors, k, ftr_num, clusterSizes, belongsTo);


	return belongsTo;




}
