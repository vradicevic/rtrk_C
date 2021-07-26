#include "k_mean_clustering.h"
uint8_t FTR_ANGLE = 0;
uint8_t calculateVarianceOfAngle(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k) {
    volatile uint16_t i, j;
    float sum = 0;
    float sum2 = 0;
    float result = 999999.99;
    uint8_t winner = 0;
    for (j = 0; j < k; j++) {
        sum = 0;
        sum2 = 0;
        if (clusterSizes[j] > 5) {
            for (i = 0; i < numOfVectors; i++) {
                if (j == belongsTo[i]) {
                    //Vps_printf("Usao");
                    sum2 += (float)vectors[5][i];
                    //sum+=CAST_ANGLE_DIS_A15(abs((int16_t)(means[j][5]-vectors[5][i]))%360);
                    //sum+=abs(means[j][4] - vectors[4][i]);
                }
            }
            sum2 = sum2 / clusterSizes[j];
            for (i = 0; i < numOfVectors; i++) {
                if (j == belongsTo[i]) {
                    //Vps_printf("Usao");
                    //sum2+=vectors[5][i];
                    sum += (float)CAST_ANGLE_DIS(abs((int16_t)(sum2 - vectors[5][i])) % 360);
                    //sum+=abs(means[j][4] - vectors[4][i]);
                }
            }
            sum = sum / clusterSizes[j];
            if (sum < result) {
                result = sum;
                winner = j;
            }

        }

    }
    return winner;


}
uint8_t calculateNexBestVariance(float** means, int16_t** vectors, uint16_t* clusterSizes, uint8_t* belongsTo, uint16_t numOfVectors, uint8_t k, uint8_t toIgnore) {
    volatile uint16_t i, j;
    float sum = 0;
    float sum2 = 0;
    float result = 999999.99;
    uint8_t winner = 0;
    for (j = 0; j < k; j++) {
        sum = 0;
        sum2 = 0;
        if (j != toIgnore) {
            if (clusterSizes[j] > 5) {
                for (i = 0; i < numOfVectors; i++) {
                    if (j == belongsTo[i]) {
                        //Vps_printf("Usao");
                        sum2 += (float)vectors[5][i];
                        //sum+=CAST_ANGLE_DIS_A15(abs((int16_t)(means[j][5]-vectors[5][i]))%360);
                        //sum+=abs(means[j][4] - vectors[4][i]);
                    }
                }
                sum2 = sum2 / clusterSizes[j];
                for (i = 0; i < numOfVectors; i++) {
                    if (j == belongsTo[i]) {
                        //Vps_printf("Usao");
                        //sum2+=vectors[5][i];
                        sum += (float)CAST_ANGLE(abs((int16_t)(sum2 - vectors[5][i])) % 360);
                        //sum+=abs(means[j][4] - vectors[4][i]);
                    }
                }
                sum = sum / clusterSizes[j];
                if (sum < result) {
                    result = sum;
                    winner = j;
                }

            }
        }

    }
    printf("Varijanca = %f; ClusterSize: %d; Svi clusteri: %d %d %d %d", result, clusterSizes[winner], clusterSizes[0], clusterSizes[1], clusterSizes[2], clusterSizes[3]);
    return winner;


}



void findMinMaxColumns(float* minima, float* maxima, uint8_t ftr_num, int16_t** vectorsFiltered, uint16_t countOfFilteredVectors) {
    volatile int index = 0;

    while (index < ftr_num) {
        minima[index] = (float)INT_MAX;
        maxima[index] = (float)-INT_MAX;
        index++;
    }

    volatile int ftr_i = 0;
    for (index = 0; index < countOfFilteredVectors; index++) {
        for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
            if ((float)vectorsFiltered[ftr_i][index] < minima[ftr_i]) {
                minima[ftr_i] = (float)vectorsFiltered[ftr_i][index];

            }
            if ((float)vectorsFiltered[ftr_i][index] > maxima[ftr_i]) {
                maxima[ftr_i] = (float)vectorsFiltered[ftr_i][index];
            }
        }
    }

}

float getRandom(float* minima, float* maxima) {
    srand(time(0));
    float result = fmod(rand(), (((*maxima) - 1) - ((*minima) + 1) + 1) + ((*minima) + 1));
    return result;
}

void initializeMeans(float** means, uint8_t k, uint8_t ftr_num, float* minima, float* maxima) {
    /*
    means- onoliko koliko je k*ftr_num
    */
    volatile int i, ftr_i;
    float inter;
    for (i = 0; i < k; i++) {
        for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
			means[i][ftr_i] = getRandom(&minima[ftr_i], &maxima[ftr_i]);

            switch (i) {
            case 0: {

                inter = minima[ftr_i];
                means[i][ftr_i] = inter;
                break;


            }
            case 1: {

                inter = maxima[ftr_i];
                means[i][ftr_i] = inter;
                break;

            }
            case 2: {

                inter = abs(maxima[ftr_i] - minima[ftr_i]);
                inter = minima[ftr_i] + inter;
                means[i][ftr_i] = inter;
                break;

            }
            case 3: {

                inter = abs(maxima[ftr_i] - minima[ftr_i]);
                means[i][ftr_i] = minima[ftr_i] + (inter / 2);
                break;

            }
            case 4: {
                inter = abs(maxima[ftr_i] - minima[ftr_i]);
                means[i][ftr_i] = maxima[ftr_i] - (inter / 2);
                break;

            }
            case 5: {
                inter = abs(maxima[ftr_i] - minima[ftr_i]);
                means[i][ftr_i] = inter;
                break;
            }
			
            }

        }
    }

}

float minkowskiDistance(int16_t** vectors, int v_index, float* mean, uint8_t ftr_num) {
	float r=3;
	float sum = 0;
	uint8_t ftr_i = 0;
	for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
		sum += pow(abs(vectors[ftr_i][v_index] - mean[ftr_i]), r);
	}
	return pow(sum, 1.0 / r);
}



float euclideanDistance(int16_t** vectors, int v_index, float* mean, uint8_t ftr_num) {
    float sum = 0;
    uint8_t ftr_i=0;
    for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
        if (ftr_i == FTR_ANGLE) {
            sum += pow(CAST_ANGLE_DIS(abs((int16_t)(mean[ftr_i] - vectors[ftr_i][v_index])) % 360), 2);
        }
        else {
            sum += pow((float)vectors[ftr_i][v_index] - mean[ftr_i], 2);
        }
		

    }
    return sqrt(sum);
}
void updateMean(uint16_t clusterSize, float* mean, int16_t** vectors, int v_index, uint8_t ftr_num) {
    volatile int ftr_i;
    float newMean;
	
    for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {

        newMean = mean[ftr_i];
        newMean = (newMean * (clusterSize - 1) + (float)vectors[ftr_i][v_index]) / clusterSize;
		
        mean[ftr_i] = newMean;
    }
}
int classify(float** means, int16_t** vectors, int v_index, uint8_t k, uint8_t ftr_num) {
    float minimum = (float)INT_MAX;

    int index = -1;
    volatile int i;
    float dis;
    for (i = 0; i < k; i++) {
        dis = minkowskiDistance(vectors, v_index, *(means + i), ftr_num);

        if (dis < minimum) {
            minimum = dis;
            index = i;
        }
    }
	if (index < 0) {
		printf("index je manji od nula");
	}
    //Vps_printf("Index = %d",index);
    return index;

}
void calculateMeans(float** means, uint8_t k, uint8_t ftr_num, int16_t** vectors, uint16_t countOfVectors, int maxIterations, uint8_t* belongsTo, uint16_t* clusterSizes, float* minima, float* maxima,int ftr_angle) {
	FTR_ANGLE = ftr_angle;
    //Vps_printf("Alocirani minima i maxima");
    volatile int i, j;
    int index;
    int cSize;
    findMinMaxColumns(minima, maxima, ftr_num, vectors, countOfVectors);

    initializeMeans(means, k, ftr_num, minima, maxima);

    for (i = 0; i < k; i++) {
        clusterSizes[i] = 0;
    }

    for (i = 0; i < countOfVectors; i++) {
        belongsTo[i] = 0;
    }
	
    volatile int noChange = 1;
    for (j = 0; j < maxIterations; j++) {
        noChange = 1;
        for (i = 0; i < k; i++) {
            clusterSizes[i] = 0;
        }
        for (i = 0; i < countOfVectors; i++) {

            index = classify(means, vectors, i, k, ftr_num);

            
            //if(cSize>countOfVectors){
            //  
            //  if (a)
            //  {
            //    a=0;
            //    Vps_printf("u kalkulejt mins %d %f %f %f %f %f",cSize,means[index][0],means[index][1],means[index][2],means[index][3],means[index][4]);
            //  }
            //  
            //}
			if (j == 0) {
				clusterSizes[index] += 1;
				cSize = clusterSizes[index];

				updateMean(cSize, *(means + index), vectors, i, ftr_num);
				noChange = 0;
			}else if (index != belongsTo[i]) {
				clusterSizes[index] += 1;
				cSize = clusterSizes[index];
				updateMean(cSize, *(means + index), vectors, i, ftr_num);
				noChange = 0;
			}
			


            
            belongsTo[i] = (uint8_t)index;

        }
		
        if (noChange) {
            break;
        }
    }
	


}
void findClusters(float** means, int16_t** vectors, uint16_t countOfVectors, uint8_t k, uint8_t ftr_num,uint16_t* clusterSizes, uint8_t* clusters) {
    volatile int i;
    for (i = 0; i < k; i++) {
        clusterSizes[i] = 0;
    }
    for (i = 0; i < countOfVectors; i++) {
        clusters[i] = (uint8_t)classify(means, vectors, i, k, ftr_num);
        clusterSizes[clusters[i]] += 1;
    }
}
