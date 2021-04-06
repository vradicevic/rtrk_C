#ifndef _EVALUATION_PROCES_H_
#define _EVALUATION_PROCES_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "read_write_utils.h"
#include "bma_utils.h"
#include "ebma.h"
#include "tss.h"
#include "my_bma.h"
#include "hibrid_bma.h"

void evaluate(char* dataFolderPath, char* resultsFolderPath, int16_t** vectors);




#endif