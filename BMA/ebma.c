#include "ebma.h"
#include <time.h>

int blockMatchingEBMA(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea, Point start, Point end) {
    Point prevMacroblockCoo;
    int deviation, x, y;
	int saeTreshold = 9*BLOCK_ELEMENTS;
    Point privFrom, privTo;
    

    int index = 0;
    int ctr = 0;
    
    for (y = start.y; y < (end.y - (end.y % BLOCK_SIZE)); y += BLOCK_SIZE) {
        for (x = start.x; x < end.x; x += BLOCK_SIZE) {
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
                
                privTo = getBestMatchEBMA(prevMacroblockCoo, currentFrame, prevFrame, searchArea, saeTreshold);
                


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
    
	int SAE = 0;
	int minSae = 99999;

    best = prevMacroblockCoo;

    currentMacroblockCoo.x = prevMacroblockCoo.x;
    currentMacroblockCoo.y = prevMacroblockCoo.y;
    //SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
    
	SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
    
    if (SAE < minSae) {
        //minSae = SAE;
		minSae = SAE;
        best = currentMacroblockCoo;
    }
    
    for (y = pointsSA[0].y; y < pointsSA[1].y; y++) {
        for (x = pointsSA[0].x; x < pointsSA[1].x; x++) {
            
            currentMacroblockCoo.x = x;
            currentMacroblockCoo.y = y;

            //SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
			SAE = calculateSAE(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);
			if (SAE < minSae) {
                //minSae = SAE;
				//printf("MIN SAE = %d\n", SAE);
				minSae = SAE;
                best = currentMacroblockCoo;
            }
            
            
        }

    }
    
    if (minSae >= saeTreshold) {
        best = prevMacroblockCoo;
	}
	
    return best;
}