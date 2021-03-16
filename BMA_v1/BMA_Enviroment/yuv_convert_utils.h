#ifndef _YUV_CONVERT_UTILS_H_
#define _YUV_CONVERT_UTILS_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void convertYUV444ToYUYV422(uint8_t* yuv444, uint8_t* yuyv, int width, int height);

#endif