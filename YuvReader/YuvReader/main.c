#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

#define WIDTH 1280
#define HEIGHT 720

int main()
{
	char* fileName = "D:\\VISION_SDK_02_12_01_00\\vision_sdk\\tools\\network_tools\\bin\\captureYUV422P.yuv";
	
	int8_t blockSize = 8;
	int8_t step = 11;
	uint32_t frameSize = (WIDTH * HEIGHT * 2); //YUV422 format
	//uint32_t frameSize = (WIDTH * HEIGHT * 3)/2; //YUV420 format
	char *WfileName= "C:\\Users\\rvalentin\\Desktop\\vectors.bin";
	
	FILE * fp2;

	fp2 = fopen(WfileName, "wb");



	uint8_t* buffer;
	buffer = getFrameFromVideo(fileName, frameSize, 0);
	uint8_t* prevBufferY;
	uint8_t* currentBufferY;
	printf("One Match: %d\nMatch komponent size:%d",sizeof(Point),sizeof(int));
	prevBufferY = getYComponent_YUV422_YUYV(buffer, frameSize, WIDTH, HEIGHT);
	buffer = getFrameFromVideo(fileName, frameSize, 1);
	currentBufferY = getYComponent_YUV422_YUYV(buffer, frameSize, WIDTH, HEIGHT);
	int countOfMatches = (WIDTH/blockSize)*(HEIGHT/blockSize);
	Point* matches = malloc(countOfMatches*sizeof(Point));
	blockMatchingTSS(matches, currentBufferY, prevBufferY, blockSize, step, WIDTH, HEIGHT);
	int index = 0;
	fwrite(matches, 8, countOfMatches, fp2);
	
	free(matches);

	///fwrite(bufferY, WIDTH*HEIGHT, 1,  fp2);

	fclose(fp2);

	return 0;
	//printf("Hello World!\n");

}
