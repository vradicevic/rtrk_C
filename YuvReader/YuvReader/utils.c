#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"



uint8_t* getYComponent_YUV422_YUYV(uint8_t* image, uint32_t frameSize, int16_t width, int16_t height) {
	int index = 0;
	int flag = 1;
	int ys = 0;
	uint8_t* bufferY = malloc(width * height);
	while (index<frameSize) {
		if (flag == 1) {
			bufferY[ys] = image[index];
			ys++;
			flag = 0;
		}else {
			flag = 1;
		}
		index++;
	}
	return bufferY;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t* getFrameFromVideo(char* fileForRead, int frameSize, int frameNumber) {//prvi frame ima frameNumber = 0
	uint8_t* frame = malloc(frameSize);
	FILE* file = fopen(fileForRead, "rb");
	fseek(file, frameNumber * frameSize, 0);
	fread(frame, 1, frameSize, file);
	fclose(file);
	
	return frame;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Point getBlock(uint8_t* image, int width, int height, int blockSize, Point blockMiddle, uint8_t* blockBuffer) {//image je buffer Y komponente; fromBlock ima koordinate središta bloka
	int index = 0;
	
	////pomicanje na gornji lijevi rub makrobloka
	blockMiddle.x -= (blockSize / 2);
	blockMiddle.y -= (blockSize / 2);

	////provjera margina
	if (blockMiddle.x < 0) { blockMiddle.x = 0; }
	if (blockMiddle.x > (width - 8)) { blockMiddle.x = width - 9; }
	if (blockMiddle.y < 0) {blockMiddle.y = 0;}
	if (blockMiddle.y > (height - 8)) { blockMiddle.y = height - 9; }


	/////kopiranje elemenata makrobloka
	for (int y = 0; y < blockSize; y++) {
		for (int x = 0; x < blockSize; x++) {
			blockBuffer[index] = image[((y + blockMiddle.y) * width)+ (x + blockMiddle.x)];
			index++;
		}
	}
	/////zbog slucaja da u provvjeri margina dodje do promjene koordinate bloka, potrebno je vratiti koordinate i prethodno ih postaviti na srediste tog makrobloka
	blockMiddle.x += (blockSize / 2);
	blockMiddle.y += (blockSize / 2);
	return blockMiddle;
	
}

float calculateMAD(uint8_t* AblockBuffer, uint8_t* BblockBuffer, int blockSize) {
	
	float sum=0;
	for (int i = 0; i < (blockSize * blockSize); i++) {
		sum = sum + (fabs((float)AblockBuffer[i] - (float)BblockBuffer[i]));
	}
	
	return (sum / (blockSize * blockSize));
	
}


Point getBestMatch(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize, int step, int WIDTH, int HEIGHT){
	Point best,priv;
	Point points[9];
	int size = blockSize * blockSize;
	uint8_t currentMacroblockBuffer[8*8];
	//////postavljanje na sredinu makrobloka
	points[0].x = macroblockCoords.x + (blockSize/2);
	points[0].y = macroblockCoords.y + (blockSize/2);
	
	float minMad=99999.999;
	float MAD;
	best = points[0];
	while (step >= 1) {
		
		points[1].x = points[0].x;				//up
		points[1].y = points[0].y - step;		
		points[2].x = points[0].x;				//down
		points[2].y = points[0].y + step;
		points[3].x = points[0].x - step;			//left
		points[3].y = points[0].y;
		points[4].x = points[0].x + step;			//right
		points[4].y = points[0].y;
		points[5].x = points[0].x - step;		//upperLeft
		points[5].y = points[0].y - step;
		points[6].x = points[0].x + step;		//upperRight
		points[6].y = points[0].y - step;
		points[7].x = points[0].x - step;		//lowerLeft
		points[7].y = points[0].y + step;
		points[8].x = points[0].x + step;		//lowerRight
		points[8].y = points[0].y + step;
		for (int i = 0; i < 9; i++) {
			priv = getBlock(frameCurrent, WIDTH, HEIGHT, blockSize, points[i], currentMacroblockBuffer);
			MAD = calculateMAD(refMacroblock, currentMacroblockBuffer,blockSize);
			
			if (MAD < minMad&& MAD<0.05) {
				minMad = MAD;
				
				points[0] = priv;
				best = priv;
			}
		}
		step = step / 2;

	}
	
	///////// vracanje na gornji lijevi ugao makrobloka
	best.x -= (blockSize / 2);
	best.y -= (blockSize / 2);
	
	return best;
}


Point* blockMatchingTSS(Point* matches,uint8_t* currentFrame, uint8_t* prevFrame, int blockSize, int step, int WIDTH, int HEIGHT) {
	uint8_t* bufferMacroblock = malloc(blockSize * blockSize);
	int macroblocksCount = (WIDTH / blockSize) * (HEIGHT / blockSize);
	Point point;
	Point best;///debug varijabla 
	
	int index = 0;
	for (int y = 0; y < HEIGHT; y += blockSize) {
		for (int x = 0; x <WIDTH; x +=blockSize) {
			point.x = x;
			point.y = y;
			getBlock(prevFrame, WIDTH, HEIGHT, blockSize,point, bufferMacroblock);
			
			matches[index] = getBestMatch(point, bufferMacroblock, currentFrame, blockSize, step, WIDTH, HEIGHT);
			//printf("(%d,%d)", matches[index].x, matches[index].y);
			index++;
			
			
		}
	}
	
	free(bufferMacroblock);
	return matches;
}