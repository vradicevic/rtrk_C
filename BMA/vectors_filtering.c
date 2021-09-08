#include "vectors_filtering.h"

int copyCluster(int16_t** src, int16_t** dest, uint8_t* belongsTo, uint8_t ftr_num, int cluster_i, int v_s, uint8_t interVsOffset) {
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
	return z;
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
void appendVectors(int16_t** dest, int16_t** src, int numOfMatches,int offset) {
	for (int i = 0; i < numOfMatches; i++) {
		dest[0][offset+i] = src[0][i];
		dest[1][offset + i] = src[1][i];
		dest[2][offset + i] = src[2][i];
		dest[3][offset + i] = src[3][i];
		dest[4][offset + i] = src[4][i];
		dest[5][offset + i] = src[5][i];
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


int16_t** filterByLength(int16_t** vectors, int16_t** filteredVectors, int16_t* numOfMatches,int ftr_num) {
	int maxMatches = 14400;
	

	
	int16_t index = 0;
	for (int o = 0; o < *numOfMatches; o++) {
		if (vectors[4][o] > 2 && vectors[4][o]< 50) {
			for (int j = 0; j < ftr_num; j++) {
				filteredVectors[j][index] = vectors[j][o];

			}
			

			index++;
		}
	}
	
	
	*numOfMatches = index;
	return filteredVectors;
}

int16_t** filterVsAndBsByLength(int16_t** vectors, int16_t** filteredVectors, int* numOfMatches, int ftr_num, uint8_t* belongsTo) {
	int maxMatches = 14400;



	int index = 0;
	for (int o = 0; o < *numOfMatches; o++) {
		if (vectors[4][o] > 2 && vectors[4][o] < 50) {
			for (int j = 0; j < ftr_num; j++) {
				filteredVectors[j][index] = vectors[j][o];

			}
			belongsTo[index] = belongsTo[o];

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
		copyCluster(vectors, interVectors, belongsTo, ftr_num, i, temp, 0);
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
float getFtrMeanOfK(int16_t* ftr,uint8_t* belongsTo, int count,int cluster_i, uint8_t isAngle) {
	int sum=0;
	float result = 0;
	int size = 0;
	float y_part = 0, x_part = 0;
	int i;

	
	for (i = 0; i < count; i++) {
		if (cluster_i == belongsTo[i]) {
			if (isAngle) {
				x_part += cos((float)ftr[i] * PI / 180);
				y_part += sin((float)ftr[i] * PI / 180);
			}
			else {
				sum += ftr[i];
			}
			
			size++;
		}
		
	}
	
	if (isAngle) {
		result  = CAST_ANGLE( (atan2(y_part / size, x_part / size) * 180 / PI));
	}
	else {
		result = sum / (float)size;
	}
	
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
    copyCluster(vectors, interVectors,  belongsTo, ftr_num, cluster_i, num, 0);
    
    copyCluster(vectors, interVectors,  belongsTo, ftr_num, cluster_i2, num, clusterSizes[cluster_i]);
    
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
	copyCluster(vectors, interVectors, belongsTo, ftr_num, first, num, 0);
	copyCluster(vectors, interVectors,  belongsTo, ftr_num, second, num, clusterSizes[first]);
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
		copyCluster(vectors, interVectors, belongsTo, ftr_num, i, temp, 0);
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
		copyCluster(interVectors, cumulativeVectors, tempBelongsTo, 6, best, temp, cummulativeSize);
		
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
		copyCluster(vectors, interVectors,  belongsTo, ftr_num, i, temp, 0);
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
		copyCluster(interVectors, cumulativeVectors,  tempBelongsTo, 6, best, temp, cummulativeSize);

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

float getAngleDeviation(float mean, int16_t* items, int clusterSize)
{
	int i;
	float sum = 0;
	for (i = 0; i < clusterSize; i++) {
		sum += CAST_ANGLE_DIS(abs(mean - items[i]) % 360);
	}


	return (sum/(clusterSize*180))*100;
}


uint8_t* filterNewMethod1(int16_t** vectors, int* numOfMatches) {

	int maxMatches = 14400;
	int j;
	int cluster_i, cluster_i2;
	int16_t** items;
	int16_t** cumulativeVectors;
	int16_t** cluster;
	float** means;
	uint8_t* belongsTo;
	uint8_t* tempBelongsTo;
	float* minima;
	float* maxima;
	uint8_t k = 10;
	int ftr_num = 6;
	int cummNumOfVs = 0;
	uint16_t clusterSizes[16];
	uint16_t tempClusterSizes[16];
	uint8_t angle_ftr_num;
	means = (float**)malloc(k * sizeof(float*));
	volatile int i;
	for (i = 0; i < k; i++) {
		means[i] = (float*)malloc(ftr_num * sizeof(float));

	}

	belongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));
	tempBelongsTo = (uint8_t*)malloc(maxMatches * sizeof(uint8_t));


	minima = (float*)malloc(ftr_num * sizeof(float));


	maxima = (float*)malloc(ftr_num * sizeof(float));


	items = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));


	cluster = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));

	for (i = 0; i < 6; i++) {
		cluster[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	cumulativeVectors = (int16_t * *)malloc(ftr_num * sizeof(int16_t*));
	
	for (i = 0; i < ftr_num; i++) {
		cumulativeVectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}

	//Grupiraj prema kutu i duljini
	k = 3;
	
	ftr_num = 4;
	items[0] = vectors[0];
	items[1] = vectors[1];
	items[2] = vectors[4];
	items[3] = vectors[5];
	angle_ftr_num = 3;

	calculateMeans(means, k, ftr_num, items, *numOfMatches, 25, belongsTo, clusterSizes, minima, maxima, angle_ftr_num);
	findClusters(means, items, *numOfMatches, k, ftr_num, clusterSizes, belongsTo);
	
	//printf("\nCluster sizes: %d; %d; %d; %d;\n", clusterSizes[0], clusterSizes[1], clusterSizes[2], clusterSizes[3]);
	for (int j = 0; j < k; j++) {
		for (int i = 0; i < ftr_num; i++) {
			if (clusterSizes[j] == 0) {
				means[j][i] = 0;
			}
			else {

				if (i == 3) {
					means[j][i] = getFtrMeanOfK(items[i], belongsTo, *numOfMatches, j, 1);
				}
				else {
					means[j][i] = getFtrMeanOfK(items[i], belongsTo, *numOfMatches, j, 0);
				}

			}

		}
	}
	


	int clusterSize;
	ftr_num = 6;
	uint8_t first_second_Similarity = 0;
	uint8_t second_third_Similarity = 0;
	uint8_t first_third_Similarity = 0;
	first_second_Similarity = checkSimilarity(means[0], means[1]);
	first_third_Similarity = checkSimilarity(means[0], means[2]);
	second_third_Similarity = checkSimilarity(means[1], means[2]);
	
	
	clusterSize = copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 0, *numOfMatches, 0);
	cummNumOfVs = clusterSizes[0];
	clusterSizes[0] = clusterSize;
	memset(tempBelongsTo, 0, cummNumOfVs);
	k = 1;
	if (first_second_Similarity * first_third_Similarity * second_third_Similarity) {
		memset(tempBelongsTo, 0, *numOfMatches);
		copyCluster(vectors, cumulativeVectors, belongsTo, 6, 1, *numOfMatches, cummNumOfVs);
		cummNumOfVs += clusterSizes[1];
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 2, *numOfMatches, cummNumOfVs);
		cummNumOfVs += clusterSizes[2];
		memset(tempBelongsTo, 0, cummNumOfVs);
		clusterSizes[0] = cummNumOfVs;
		//printf("All similar %d %d %d \n",clusterSizes[0],clusterSizes[1],clusterSizes[2]);
	}
	else if (first_second_Similarity) {
		//kopiraj drugi klaster u prvi klaster
		copyCluster(vectors, cumulativeVectors, belongsTo, 6, 1, *numOfMatches, cummNumOfVs);
		cummNumOfVs += clusterSizes[1];
		clusterSizes[0] = cummNumOfVs;
		memset(tempBelongsTo, 0, cummNumOfVs);
		//kopiraj treći klaster u drugi klaster
		copyCluster(vectors, cumulativeVectors, belongsTo, 6, 2, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo+cummNumOfVs, 1, clusterSizes[2]);
		cummNumOfVs += clusterSizes[2];
		clusterSizes[1] = clusterSizes[2];
		//printf("First and second %d %d %d\n", clusterSizes[0], clusterSizes[1], clusterSizes[2]);
		k = 2;
	}
	else if (first_third_Similarity) {
		//kopiraj treći klaster u prvi
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 2, *numOfMatches, cummNumOfVs);
		cummNumOfVs += clusterSizes[2];
		memset(tempBelongsTo, 0, cummNumOfVs);
		clusterSizes[0] = cummNumOfVs;

		//kopiraj drugi klaster u drugi klaster
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 1, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo + cummNumOfVs, 1, clusterSizes[1]);
		cummNumOfVs += clusterSizes[1];
		clusterSizes[1] = clusterSizes[1];
		//printf("First and third\n");
		k = 2;


	}
	else if(second_third_Similarity){
		//kopiraj drugi klaster u drugi klaster
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 1, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo + cummNumOfVs, 1, clusterSizes[1]);
		cummNumOfVs += clusterSizes[1];

		//kopiraj treći klaster u drugi klaster
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 2, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo + cummNumOfVs, 1, clusterSizes[2]);
		cummNumOfVs += clusterSizes[2];
		clusterSizes[0] += clusterSizes[2];
		//printf("third and second\n");
		k = 2;

	}
	else {
		//kopiraj drugi klaster u drugi klaster
		copyCluster(vectors, cumulativeVectors, belongsTo, 6, 1, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo + cummNumOfVs, 1, clusterSizes[1]);
		cummNumOfVs += clusterSizes[1];
		//kopiraj treći klaster u treći klaster
		copyCluster(vectors, cumulativeVectors,  belongsTo, 6, 2, *numOfMatches, cummNumOfVs);
		memset(tempBelongsTo + cummNumOfVs, 2, clusterSizes[2]);
		cummNumOfVs += clusterSizes[2];
		k = 3;
		

	}
	items[0] = cumulativeVectors[0];
	items[1] = cumulativeVectors[1];
	items[2] = cumulativeVectors[4];
	items[3] = cumulativeVectors[5];
	ftr_num = 4;
	for (j = 0; j < k; j++) {
		for (int i = 0; i < ftr_num; i++) {
			if (clusterSizes[j] == 0) {
				means[j][i] = 0;
			}
			else {
				
				if (i == 3) {
					means[j][i] = getFtrMeanOfK(items[i], tempBelongsTo, cummNumOfVs, j, 1);
				}
				else {
					means[j][i] = getFtrMeanOfK(items[i], tempBelongsTo, cummNumOfVs, j,0);
				}

			}

		}
	}
	copyVectors(vectors, cumulativeVectors, cummNumOfVs);
	copyBelongsTo(tempBelongsTo, belongsTo, cummNumOfVs);
	
	float var;
	uint8_t reCluster[3];
	for (j = 0; j < k; j++) {
		//izračunaj varijancu kuta za cluster
		reCluster[j] = 0;
		int clusterSize = copyCluster(cumulativeVectors, cluster, tempBelongsTo, 6, j, cummNumOfVs, 0);
		clusterSizes[j]= clusterSize;
		if (clusterSize != 0) {
			var = getAngleDeviation(means[j][3], cluster[5], clusterSize);

			//printf("Moguće posto %f\n", var);
			if (var > 35.0) {
				reCluster[j] = 1;
			}
			
		}
		

	}

	

	*numOfMatches = cummNumOfVs;
	ftr_num = 1;
	angle_ftr_num = 0;	
	uint8_t newK = 0;
	
	cummNumOfVs = 0;
	for(j = 0; j < k; j++) {
		if (reCluster[j]) {
			clusterSize = copyCluster(vectors, cluster, belongsTo, 6, j, *numOfMatches, 0);
			items[0] = cluster[5];
			copyVectors(cumulativeVectors, cluster, clusterSize);

			calculateMeans(means, 2, ftr_num, items, clusterSize, 25, tempBelongsTo, tempClusterSizes, minima, maxima, angle_ftr_num);
			findClusters(means, items, clusterSize, 2, ftr_num, tempClusterSizes, tempBelongsTo);
			memset(tempBelongsTo + cummNumOfVs, newK, tempClusterSizes[0]);
			newK++;
			cummNumOfVs += tempClusterSizes[0];
			memset(tempBelongsTo + cummNumOfVs, newK, tempClusterSizes[1]);
			newK++;
			cummNumOfVs += tempClusterSizes[1];
			

		}
		else {
			clusterSize= copyCluster(vectors, cumulativeVectors, belongsTo, 6, j, *numOfMatches, cummNumOfVs);
			memset(tempBelongsTo+ cummNumOfVs, newK, clusterSizes[j]);
			cummNumOfVs += clusterSizes[j];
			newK++;
			//printf("NewK %d, %d, %d\n", newK,k,clusterSizes[j]);

		}
		
	}

	copyVectors(vectors, cumulativeVectors, cummNumOfVs);
	copyBelongsTo(tempBelongsTo, belongsTo, cummNumOfVs);
	
	
	



	return belongsTo;

}

uint8_t checkSimilarity(float* meanFirst, float* meanSecond) {//mean je u ovome UC-u složen kao [0] - x ishodista, [1] - y ishodissta, [2] - duljina, [3] - kut
	
	uint8_t angleSim = 0, lengthSim = 0, locationSim = 0;
	int angleDis = CAST_ANGLE_DIS(abs(meanFirst[3] - meanSecond[3]) % 360);
	float lengthDis = abs(meanFirst[2] - meanSecond[2]);
	float locationDis = helperEuclidDis(meanFirst[0], meanFirst[1], meanSecond[0], meanSecond[1]);
	if (angleDis < 40) {
		angleSim = 1;
	}
	if (lengthDis < 10) {
		lengthSim = 1;
	}
	if (locationDis <0.25*WIDTH) {
		locationSim = 1;
	}
	return (uint8_t)(angleSim * lengthSim* locationSim);
}

float helperEuclidDis(float x1, float y1, float x2, float y2) {

	float sum;
	
	sum = pow(fabs(x1 - x2), 2) + pow(fabs(y1 - y2), 2);
	
	return sqrt(sum);


}

void nullDistancedVectorsInClusters(int16_t** vectors,float** means, uint8_t* belongsTo, int numOfVs) {//mean je u ovome UC-u složen kao [0] - x ishodista, [1] - y ishodissta, [2] - duljina, [3] - kut
	
	int dev = 0;
	for (int i = 0; i < numOfVs; i++) {
		dev = CAST_ANGLE_DIS(abs((int16_t)means[belongsTo[i]][3] - vectors[5][i]));
		printf("distance of angles %d\n", dev);
		if (dev > 150) {
			//vectors[4][i] = 0;
		}
	}

}

