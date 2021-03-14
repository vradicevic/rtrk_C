#ifndef _READ_WRITE_UTILS_H_
#define _READ_WRITE_UTILS_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t* readFileYUV422_YUYV(char* filepath, int width, int height);

uint8_t* readFileYUV420P_YpUVp(char* filepath, int width, int height);

uint8_t* readFileYUV444_YUV(char* filepath, int width, int height);

void getYComponent_YUV422_YUYV(uint8_t* Y, uint8_t* image, int width, int height);

void getYComponent_YUV420P_YpUVp(uint8_t* Y, uint8_t* image, int width, int height);

void getYComponent_YUV444_YUV(uint8_t* Y, uint8_t* image, int width, int height);

uint8_t* readFrameFrom444YUVVideo(char* filepath, int width, int height, int frameNumber);

uint8_t* readFrameFrom422YUYVVideo(char* filepath, int width, int height, int frameNumber);


#endif