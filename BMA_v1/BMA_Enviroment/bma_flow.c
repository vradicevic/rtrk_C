#include "bma_flow.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "read_write_utils.h"
#include "ebma.h"
#include "tss.h"
#include "my_bma.h"
#include "hibrid_bma.h"
#include "paths.h"


int bmaFlow(int step, int16_t** vectors) {
	Point start, end;
	clock_t beginClock, endClock;
	start.x = 0;
	end.x = WIDTH;
	start.y = 0;
	end.y = HEIGHT;
	int ftr_num = 6;
	int maxMatches = (HEIGHT / BLOCK_SIZE) * (WIDTH / BLOCK_SIZE);
	uint8_t* image;
	uint8_t* prevFrame = (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	uint8_t* currFrame= (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	char* videoPath = videoMirnaCenter30FPS;
	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT,12);
	getYComponent_YUV422_YUYV(prevFrame, image, WIDTH, HEIGHT);
	free(image);
	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT, 13);
	getYComponent_YUV422_YUYV(currFrame, image, WIDTH, HEIGHT);
	 
	beginClock = clock();
	int numMatches = blockMatchingEBMA(vectors, currFrame, prevFrame, step, start, end);
	endClock = clock();
	printf("exTime: %lf", (double)(endClock - beginClock)/CLOCKS_PER_SEC);
	printf("BlockMatching done!\n");
	return numMatches;
}