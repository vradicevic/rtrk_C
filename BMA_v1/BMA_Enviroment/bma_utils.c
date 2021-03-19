
#include "bma_utils.h"

int calculateLength2Points(Point from, Point to) {
    int length = sqrt((pow((abs(to.x) - abs(from.x)), 2) + pow((abs(to.y) - abs(from.y)), 2)));
    return length;
}
int calculateAngle2Points(Point to, Point from) {
    int result = atan2((float)(to.y - from.y), (float)(to.x - from.x)) * 180 / PI;

    return result;
}

int blockValueDeviation(uint8_t* frame, Point upperLeftMacroblockCoo) {
    int sum = 0;
    
    int y, x;
    for (y = 0; y < BLOCK_SIZE; y++) {
        for (x = 0; x < BLOCK_SIZE; x++) {
            sum = sum + frame[((y + upperLeftMacroblockCoo.y) * WIDTH) + (x + upperLeftMacroblockCoo.x)];

        }
    }
    int mean = sum / (BLOCK_SIZE * BLOCK_SIZE);
    
    sum = 0;
    for (y = 0; y < BLOCK_SIZE; y++) {
        for (x = 0; x < BLOCK_SIZE; x++) {
            sum += abs(mean - frame[((y + upperLeftMacroblockCoo.y) * WIDTH) + (x + upperLeftMacroblockCoo.x)]);
        }
    }

    return sum;
}
Point getCenter(Point upperLeft) {
    Point center;
    if (upperLeft.x > (WIDTH - BLOCK_SIZE - 1)) {
        upperLeft.x = WIDTH - BLOCK_SIZE - 1;
    }
    if (upperLeft.y > (HEIGHT - BLOCK_SIZE - 1)) {
        upperLeft.y = HEIGHT - BLOCK_SIZE - 1;
    }
    center.x = upperLeft.x + (BLOCK_SIZE / 2);
    center.y = upperLeft.y + (BLOCK_SIZE / 2);
    return center;

}

Point getUpperLeft(Point center) {
    Point upperLeft;
    if (center.x < (BLOCK_SIZE / 2)) {
        center.x = BLOCK_SIZE / 2;
    }
    if (center.y < (BLOCK_SIZE / 2)) {
        center.y = (BLOCK_SIZE / 2);
    }
    if (center.x > (WIDTH - (BLOCK_SIZE / 2))) {
        center.x = WIDTH - (BLOCK_SIZE / 2);
    }
    if (center.y > (HEIGHT - (BLOCK_SIZE / 2))) {
        center.y = HEIGHT - (BLOCK_SIZE / 2);
    }
    upperLeft.x = center.x - (BLOCK_SIZE / 2);
    upperLeft.y = center.y - (BLOCK_SIZE / 2);
    return upperLeft;
}

float calculateMAD(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo) {
    float sum = 0;
    int x, y;
    for (y = 0; y < BLOCK_SIZE; y++) {
        for (x = 0; x < BLOCK_SIZE; x++) {
            sum = sum + (fabs((float)prevFrame[((y + prevMacroblockCoo.y) * WIDTH) + (x + prevMacroblockCoo.x)] - (float)currentFrame[((y + currentMacroblockCoo.y) * WIDTH) + (x + currentMacroblockCoo.x)]));

        }
    }
    return (sum / (BLOCK_ELEMENTS));

}

float calculateSUMDIFF(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo) {
    float sumPrev = 0;
    float sumCurr = 0;
    int x, y;
    for (y = 0; y < BLOCK_SIZE; y++) {
        for (x = 0; x < BLOCK_SIZE; x++) {
            sumPrev += (float)prevFrame[((y + prevMacroblockCoo.y) * WIDTH) + (x + prevMacroblockCoo.x)];
            sumCurr += (float)currentFrame[((y + currentMacroblockCoo.y) * WIDTH) + (x + currentMacroblockCoo.x)];
        }
    }
    float sumdiff = fabs(sumPrev - sumCurr)/BLOCK_ELEMENTS;
    return sumdiff;

}

void getSearchArea(Point prevMacroblockCoo, int searchArea, Point pointsSA[2]) {

    int x = prevMacroblockCoo.x - searchArea;
    if (x < 0) { x = 0; }
    int y = prevMacroblockCoo.y - searchArea;
    if (y < 0) { y = 0; }
    pointsSA[0].x = x;
    pointsSA[0].y = y;
    x = prevMacroblockCoo.x + BLOCK_SIZE + searchArea;
    if (x > WIDTH) { x = WIDTH - BLOCK_SIZE; }
    y = prevMacroblockCoo.y + BLOCK_SIZE + searchArea;
    if (y > HEIGHT) { y = HEIGHT - BLOCK_SIZE; }
    pointsSA[1].x = x;
    pointsSA[1].y = y;

}