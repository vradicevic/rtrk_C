#ifndef _HIBRID_BMA_H_
#define _HIBRID_BMA_H_

#include "bma_utils.h"

int blockMatchingHBMA(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea, Point start, Point end);
Point getBestMatchHBMA(Point prevMacroblockCoo, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea, Point predicted);

#endif
