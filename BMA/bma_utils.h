#ifndef _BMA_UTILS_H_
#define _BMA_UTILS_H_
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265

#define WIDTH (1280)
#define HEIGHT (720)
#define BLOCK_SIZE (32)
#define BLOCK_ELEMENTS (BLOCK_SIZE*BLOCK_SIZE)
#define RANDOM(upper,lower) ((rand() % (upper - lower + 1)) + lower)
#define CAST_ANGLE(prev)(prev<0?(360+prev):prev)
#define CAST_ANGLE_DIS(prev)((prev>180)?(360-prev):prev)


typedef struct {
	int x;
	int y;
}Point;

int calculateLength2Points(Point from, Point to);

int calculateAngle2Points(Point to, Point from);

float calculateMAD(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo);

int calculateSAE(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo);

float getBiplanarDiff(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo,int median);

float calculateSUMDIFF(uint8_t* currentFrame, uint8_t* prevFrame, Point currentMacroblockCoo, Point prevMacroblockCoo);

int blockValueDeviation(uint8_t* frame, Point upperLeftMacroblockCoo);

int getMedianOfBlock(uint8_t* frame, Point upperLeftMacroblockCoo);

Point getCenter(Point upperLeft);

Point getUpperLeft(Point center);

void getSearchArea(Point prevMacroblockCoo, int searchArea, Point pointsSA[2]);

void swap(int* p, int* q);

void sort(int a[], int n);

#endif