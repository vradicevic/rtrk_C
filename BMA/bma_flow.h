#ifndef _BMA_FLOW_H_
#define _BMA_FLOW_H_
#define _CRT_SECURE_NO_WARNINGS
#include "stdint.h"

#include "bma_utils.h"



uint8_t* bmaFlow(int step, int16_t** vectors, int16_t** filteredVectors, int* numOfVs);

void mainDummyEval(void);

#endif