#include "tss.h"
int blockMatchingTSS(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int step, Point start, Point end) {
    Point prevMacroblockCoo;
    int deviation, x, y;
    Point privFrom, privTo;
    int index = 0;


    for (y =200; y < HEIGHT - (HEIGHT % BLOCK_SIZE)-200; y += BLOCK_SIZE) {
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

                privTo = getBestMatchTSS(prevMacroblockCoo, currentFrame, prevFrame, step);


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
                vectors[5][index] = CAST_ANGLE(calculateAngle2Points(privTo, privFrom));
                index++;
            //}
            //printf("(%d,%d)", matches[index].x, matches[index].y);




        }
    }

    return index;

}

Point getBestMatchTSS(Point prevMacroblockCoo, uint8_t* currentFrame, uint8_t* prevFrame, int step) {
    Point best;
    Point points[9];
    Point currentMacroblockCoo;

    /////blockSize*blockSize
    //////postavljanje na sredinu makrobloka
    points[0] = getCenter(prevMacroblockCoo);


    float minMad = 99999.999;
    float MAD;
    best = points[0];
    int i;

    while (step >= 1) {

        points[1].x = points[0].x;              //up
        points[1].y = points[0].y - step;
        points[2].x = points[0].x;              //down
        points[2].y = points[0].y + step;
        points[3].x = points[0].x - step;           //left
        points[3].y = points[0].y;
        points[4].x = points[0].x + step;           //right
        points[4].y = points[0].y;
        points[5].x = points[0].x - step;       //upperLeft
        points[5].y = points[0].y - step;
        points[6].x = points[0].x + step;       //upperRight
        points[6].y = points[0].y - step;
        points[7].x = points[0].x - step;       //lowerLeft
        points[7].y = points[0].y + step;
        points[8].x = points[0].x + step;       //lowerRight
        points[8].y = points[0].y + step;
        for (i = 0; i < 9; i++) {
            currentMacroblockCoo = getUpperLeft(points[i]);

            MAD = calculateMAD(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);

            if (MAD < minMad) {
                //printf("Iteration:%d \tMAD: %f\n", iteration, MAD);
                minMad = MAD;
                best = points[i];

            }

        }
        //printf("Iteration:%d \tMAD: %f\n", iteration, minMad);

        points[0] = best;
        /*if (minMad > 1.0 && (best.x != lastBest.x && best.y != lastBest.y)) {
            step = step;
            iteration++;
        }
        else { step = step / 2; iteration = 0; }*/
        step = step / 2;


    }

    printf("MinMAD: %f\n", minMad);
    ///////// vracanje na gornji lijevi ugao makrobloka
    best = getUpperLeft(best);
    if (minMad > 9.00) {
        best = prevMacroblockCoo;
    }
    return best;
}