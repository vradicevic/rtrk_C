#ifndef _TSS_H_
#define _TSS_H_

#include "bma_utils.h"

Point getBestMatchTSS(Point prevMacroblockCoo, uint8_t* currentFrame, uint8_t* prevFrame, int step, Point start, Point end);
int blockMatchingTSS(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int step);

#endif