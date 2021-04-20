#include "ebma.h"

int blockMatchingEBMA(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea, Point start, Point end) {
    Point prevMacroblockCoo;
    int deviation, x, y;
	int saeTreshold = 9 * (BLOCK_ELEMENTS);
    Point privFrom, privTo;
    int index = 0;
    for (y = 200; y < (HEIGHT - (HEIGHT % BLOCK_SIZE)-200); y += BLOCK_SIZE) {
        for (x = 0; x < WIDTH - ((WIDTH % BLOCK_SIZE)); x += BLOCK_SIZE) {
            prevMacroblockCoo.x = x;
            prevMacroblockCoo.y = y;
            prevMacroblockCoo = getCenter(prevMacroblockCoo);
            prevMacroblockCoo = getUpperLeft(prevMacroblockCoo);


            privFrom = prevMacroblockCoo;


            deviation = blockValueDeviation(currentFrame, prevMacroblockCoo);

            if (((float)deviation) / (BLOCK_SIZE * BLOCK_SIZE) > 5.0) {
                //printf("Deviation: %d\n", deviation);
                //float varin = ((float)deviation)/(BLOCK_SIZE*BLOCK_SIZE);
                //Vps_printf("Variance deviation %f", varin);

                privTo = getBestMatchEBMA(prevMacroblockCoo, currentFrame, prevFrame, searchArea,saeTreshold);


            }
            else {

                privTo = prevMacroblockCoo;
            }


            int length = calculateLength2Points(privFrom, privTo);
            //if (length > 2) {
                vectors[0][index] = (int16_t)privFrom.x;
                vectors[1][index] = (int16_t)privFrom.y;
                vectors[2][index] = (int16_t)privTo.x;
                vectors[3][index] = (int16_t)privTo.y;
                vectors[4][index] = (int16_t)length;
                vectors[5][index] = (int16_t)CAST_ANGLE(calculateAngle2Points(privTo, privFrom));
                index++;
            //}
            //printf("(%d,%d)", matches[index].x, matches[index].y);


        }
    }
    return index;

}

Point getBestMatchEBMA(Point prevMacroblockCoo, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea,int saeTreshold) {
    Point best;
    Point pointsSA[2];
	
    getSearchArea(prevMacroblockCoo, searchArea, pointsSA);
    Point currentMacroblockCoo;
    int x, y;
    float minMad = 99999.999;
    float MAD;
	int SAE = 0;
	int minSAE = 999999;

    best = prevMacroblockCoo;

    currentMacroblockCoo.x = prevMacroblockCoo.x;
    currentMacroblockCoo.y = prevMacroblockCoo.y;
    //MAD = calculateMAD(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
	SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);

    if (SAE < minSAE) {
        //minMad = MAD;
		minSAE = SAE;
        best = currentMacroblockCoo;
    }

    for (y = pointsSA[0].y; y < pointsSA[1].y; y++) {
        for (x = pointsSA[0].x; x < pointsSA[1].x; x++) {
            currentMacroblockCoo.x = x;
            currentMacroblockCoo.y = y;

            //MAD = calculateMAD(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
			SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
			if (SAE < minSAE) {
                //minMad = MAD;
				//printf("MIN SAE = %d\n", SAE);
				minSAE = SAE;
                best = currentMacroblockCoo;
            }
        }

    }
    
    if (minSAE >= saeTreshold) {
        best = prevMacroblockCoo;
	}
	
    return best;
}