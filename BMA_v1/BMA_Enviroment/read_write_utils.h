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

void saveVectors(char* filepath, int16_t** buff, int16_t itemsNum, int wordWidth);

void appendFrameToYUYVFile(char* filepath, uint8_t* yuyv, int width, int height);

void appendFrameToYUV444File(char* filepath, uint8_t* yuv444, int width, int height);

void saveYUYVImage(char* imagePath, uint8_t image, int width, int height);

void saveBelongsTo(char* filepath, uint8_t* buff, int16_t itemsNum);

void saveYComponent(char* filepath, uint8_t* buffY, int width, int height);

void appendYComponent(char* filepath, uint8_t* buffY, int width, int height);

#endif