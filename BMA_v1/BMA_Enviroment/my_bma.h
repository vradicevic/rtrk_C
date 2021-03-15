#ifndef _MY_BMA_H_
#define _MY_BMA_H_

#include "bma_utils.h"

int blockMatchingMYBMA(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea);
Point getBestMatchMYBMA(Point prevMacroblockCoo, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea);

#endif