#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

void filterVectorsForDuplicationError(Vector* vectors,int width, int height, int blockSize) {
	///Looking at same block
	int count = (width / blockSize) * (height / blockSize);
	int i = 0;
	int repeated = 0;
	int forCorrection[50];
	int index = 0;
	for (int y = 0; y < height-blockSize; y++) {
		for (int x = 0; x < width-blockSize; x++) {
			for (index = 0; index < count; index++) {
				if (vectors[index].to.x<(x + blockSize) && vectors[index].to.x>x) {
					if (vectors[index].to.y<(y + blockSize) && vectors[index].to.y>y) {
						repeated++;
						forCorrection[i] = index;
						i++;
					}
				}
			}
			if (repeated > 1) {
				for (int z = 0; z < (i); z++) {
		
					vectors[forCorrection[z]].to = vectors[forCorrection[z]].from;
				}
			}
			
			repeated = 0;
			i = 0;
			
		}
	}
}
void filterVectorsByOrientation(Vector* vectors, int blockSize) {
	int angles[60];
	int i = 0;
	int angle = 0;
	int countOfVectors = (WIDTH / blockSize) * (HEIGHT / blockSize);
	for (int y = 0; y < countOfVectors;y++) {
		
		for (int x = 0; x < countOfVectors; x++) {
			if ((vectors[x].angle > vectors[y].angle - 3) && (vectors[x].angle < vectors[y].angle + 3)) {
				vectors[x].angle = vectors[y].angle;
				int x_offset = vectors[x].length * (cos((double)(vectors[x].angle * PI / 180)));
				vectors[x].to.x = vectors[x].from.x + x_offset;
				int y_offset = vectors[x].length * (sin((double)(vectors[x].angle * PI / 180)));
				vectors[x].to.y = vectors[x].from.y + y_offset;

			}
		}
	}

}

int calculateLength2Points(Point from, Point to) {
	int length = sqrt((pow((abs(to.x) - abs(from.x)),2) + pow((abs(to.y) - abs(from.y)),2)));
	return length;
}
int calculateAngle2Points(Point to, Point from) {
	int result = atan2(to.y - from.y, to.x - from.x) * 180 / PI;
	return result;
}

int blockValueDeviation(uint8_t* block,int blockSize) {
	int sum = 0;
	for (int i = 0; i < (blockSize * blockSize); i++) {
		sum += block[i];
	}
	int mean = sum / (blockSize * blockSize);
	sum = 0;
	for(int i = 0; i < (blockSize * blockSize); i++) {
		sum += abs(mean - block[i]);
	}
	return sum;
}
uint8_t* getYComponent_YUV422_YUYV(uint8_t* image, uint32_t frameSize, int width, int height) {
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
uint8_t* getYComponent_YUV420P(char* filepath, int width, int height) {
	uint8_t* Ys = malloc(width * height);
	int ySize = width * height;
	FILE* file = fopen(filepath, "rb");
	fread(Ys, 1, ySize, file);
	fclose(file);
	return Ys;
}
uint8_t* getYComponent_YUV444_YUV(char* filepath,  int width, int height) {
	uint8_t* Ys = malloc(width * height);
	uint8_t* buffer = malloc(width * height * 3);
	int frameSize = width * height*3;
	FILE* file = fopen(filepath, "rb");
	fread(buffer, 1, frameSize, file);
	fclose(file);
	int index = 0;
	for (int i = 0; i < frameSize; i += 3) {
		Ys[index] = buffer[i];
		index++;
	}
	free(buffer);
	return Ys;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t* getFrameFrom422YUYVVideo(char* fileForRead, int width, int height, int frameNumber) {//prvi frame ima frameNumber = 0
	uint32_t frameSize = (width * height * 2);
	uint8_t* frame = malloc(frameSize);
	FILE* file = fopen(fileForRead, "rb");
	fseek(file, frameNumber * frameSize, 0);
	fread(frame, 1, frameSize, file);
	fclose(file);
	
	return frame;

}
uint8_t* getFrameFrom420YUVVideo(char* fileForRead, int frameSize, int frameNumber) {//prvi frame ima frameNumber = 0
	uint8_t* frame = malloc(frameSize);
	FILE* file = fopen(fileForRead, "rb");
	fseek(file, frameNumber * frameSize, 0);
	fread(frame, 1, frameSize, file);
	fclose(file);

	return frame;

}
Point getCenter(Point upperLeft) {
	Point center;
	center.x = upperLeft.x + (BLOCK_SIZE / 2);
	center.y = upperLeft.y + (BLOCK_SIZE / 2);
	return center;

}

Point getUpperLeft(Point center) {
	Point upperLeft;
	upperLeft.x = center.x - (BLOCK_SIZE / 2);
	upperLeft.y = center.y - (BLOCK_SIZE / 2);
	return upperLeft;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Point getBlock(uint8_t* image, int width, int height, int blockSize, Point currentPoint, uint8_t* blockBuffer) {//image je buffer Y komponente; fromBlock ima koordinate središta bloka
	int index = 0;
	Point blockMiddle;
	blockMiddle.x = currentPoint.x;
	blockMiddle.y = currentPoint.y;
	////pomicanje na gornji lijevi rub makrobloka
	blockMiddle.x -= (blockSize / 2);
	blockMiddle.y -= (blockSize / 2);

	////provjera margina
	if (blockMiddle.x < 0) { blockMiddle.x = 0; }
	if (blockMiddle.x > (width - blockSize)) { blockMiddle.x = width - (blockSize); }
	if (blockMiddle.y < 0) {blockMiddle.y = 0;}
	if (blockMiddle.y > (height - blockSize)) { blockMiddle.y = height - (blockSize); }


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

long float calculateMAD(uint8_t* AblockBuffer, uint8_t* BblockBuffer, int blockSize) {
	
	long float sum=0;
	for (int i = 0; i < (BLOCK_ELEMENTS); i++) {
		sum = sum + (fabs((long float)AblockBuffer[i] - (long float)BblockBuffer[i]));
	}
	
	return (sum / (BLOCK_ELEMENTS));
	
}
long float calculateMSE(uint8_t* AblockBuffer, uint8_t* BblockBuffer, int blockSize) {

	long float sum = 0;
	for (int i = 0; i < (BLOCK_ELEMENTS); i++) {
		sum +=pow(((long float)AblockBuffer[i] - (long float)BblockBuffer[i]),2);
	}

	return (sum / (BLOCK_ELEMENTS));

}

Point searcherSDSP(Point center, uint8_t* refMacroblock, uint8_t* frameCurrent, float minMAD) {
	Point* points= malloc(5*sizeof(Point));
	Point priv;
	int best = 0;
	float MAD;
	uint8_t currentMacroblockBuffer[BLOCK_SIZE * BLOCK_SIZE];
	
	init4PointsForSDSP(center, points);
	for (int i = 1; i < 5; i++) {
		priv = getBlock(frameCurrent, WIDTH, HEIGHT, BLOCK_SIZE, points[i], currentMacroblockBuffer);
		MAD = calculateMAD(refMacroblock,currentMacroblockBuffer,BLOCK_SIZE);
		if (MAD < minMAD) {
			minMAD = MAD;
			best = i;
			points[i] = priv;
			
		}
	}
	switch (best)
	{
	case 0:
		priv = points[0];
		free(points);
		return priv;
	case 1:
		priv = points[1];
		free(points);
		return searcherSDSP(priv,refMacroblock,frameCurrent,minMAD);
	case 2:
		priv = points[2];
		free(points);
		return searcherSDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 3:
		priv = points[3];
		free(points);
		return searcherSDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 4:
		priv = points[4];
		free(points);
		return searcherSDSP(priv, refMacroblock, frameCurrent, minMAD);
	}
	
}

Point searcherLDSP(Point center, uint8_t* refMacroblock, uint8_t* frameCurrent, float minMAD) {
	Point* points = malloc(9*sizeof(Point));
	Point priv;
	int best = 0;
	float MAD;
	uint8_t currentMacroblockBuffer[BLOCK_SIZE * BLOCK_SIZE];
	init9PointsForLDSP(center, points);
	
	for (int i = 1; i < 9; i++) {
		priv = getBlock(frameCurrent, WIDTH, HEIGHT, BLOCK_SIZE, points[i], currentMacroblockBuffer);
		MAD = calculateMAD(refMacroblock, currentMacroblockBuffer, BLOCK_SIZE);
		if (MAD < minMAD) {
			//printf("minMAD: %f\n",MAD);
			minMAD = MAD;
			best = i;
			points[i] = priv;
		}
	}

	switch (best)
	{
	case 0:
		priv = points[0];
		free(points);
		return searcherSDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 1:
		priv = points[1];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 2:
		priv = points[2];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 3:
		priv = points[3];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 4:
		priv = points[4];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 5:
		priv = points[5];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 6:
		priv = points[6];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 7:
		priv = points[7];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	case 8:
		priv = points[8];
		free(points);
		return searcherLDSP(priv, refMacroblock, frameCurrent, minMAD);
	}
}


void init4PointsForSDSP(Point center, Point* points) {
	points[0] = center;
	points[1].x = points[0].x;				//up
	points[1].y = points[0].y - 1;
	points[2].x = points[0].x;				//down
	points[2].y = points[0].y + 1;
	points[3].x = points[0].x - 1;			//left
	points[3].y = points[0].y;
	points[4].x = points[0].x + 1;			//right
	points[4].y = points[0].y;
	

	if (center.x < 6) {
		points[1].x = 4;
		points[2].x = 4;
		points[3].x = 4;
		points[4].x = 4;
		
	}
	if (center.x > WIDTH - 5) {
		points[1].x = WIDTH - 5;
		points[2].x = WIDTH - 5;
		points[3].x = WIDTH - 5;
		points[4].x = WIDTH - 5;
		
	}
	if (center.y < 6) {
		points[1].y = 4;
		points[2].y = 4;
		points[3].y = 4;
		points[4].y = 4;
		
	}
	if (center.y > HEIGHT - 5) {
		points[1].y = HEIGHT - 5;
		points[2].y = HEIGHT - 5;
		points[3].y = HEIGHT - 5;
		points[4].y = HEIGHT - 5;
	}
}

void init9PointsForLDSP(Point center, Point * points) {
	
	points[0] = center;
	points[1].x = points[0].x;				//up
	points[1].y = points[0].y - 2;
	points[2].x = points[0].x;				//down
	points[2].y = points[0].y + 2;
	points[3].x = points[0].x - 2;			//left
	points[3].y = points[0].y;
	points[4].x = points[0].x + 2;			//right
	points[4].y = points[0].y;
	points[5].x = points[0].x - 1;		//upperLeft
	points[5].y = points[0].y - 1;
	points[6].x = points[0].x + 1;		//upperRight
	points[6].y = points[0].y - 1;
	points[7].x = points[0].x - 1;		//lowerLeft
	points[7].y = points[0].y + 1;
	points[8].x = points[0].x + 1;		//lowerRight
	points[8].y = points[0].y + 1;

	if (center.x < 6) {
		points[1].x = 4;
		points[2].x = 4;
		points[3].x = 4;
		points[4].x = 4;
		points[5].x = 4;
		points[6].x = 4;
		points[7].x = 4;
		points[8].x = 4;
	}
	if (center.x > WIDTH-5) {
		points[1].x = WIDTH-5;
		points[2].x = WIDTH - 5;
		points[3].x = WIDTH - 5;
		points[4].x = WIDTH - 5;
		points[5].x = WIDTH - 5;
		points[6].x = WIDTH - 5;
		points[7].x = WIDTH - 5;
		points[8].x = WIDTH - 5;
	}
	if (center.y < 6) {
		points[1].y = 4;
		points[2].y = 4;
		points[3].y = 4;
		points[4].y = 4;
		points[5].y = 4;
		points[6].y = 4;
		points[7].y = 4;
		points[8].y = 4;
	}
	if (center.y > HEIGHT - 5) {
		points[1].y = HEIGHT - 5;
		points[2].y = HEIGHT- 5;
		points[3].y = HEIGHT- 5;
		points[4].y = HEIGHT- 5;
		points[5].y = HEIGHT- 5;
		points[6].y = HEIGHT- 5;
		points[7].y = HEIGHT- 5;
		points[8].y = HEIGHT- 5;
	}

}



Point getBestMatchTSS(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize, int step){
	Point best,priv;
	Point points[9];
	int size = blockSize * blockSize;
	uint8_t currentMacroblockBuffer[BLOCK_SIZE*BLOCK_SIZE];/////blockSize*blockSize
	//////postavljanje na sredinu makrobloka
	points[0].x = macroblockCoords.x + (blockSize/2);
	points[0].y = macroblockCoords.y + (blockSize/2);
	
	long float minMad=99999.999;
	long float MAD;
	best = points[0];
	Point lastBest = points[0];
	int iteration = 0;
	while (step >= 1) {
		lastBest = best;
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

			if (MAD < minMad) {
					//printf("Iteration:%d \tMAD: %f\n", iteration, MAD);
				minMad = MAD;
				best = priv;
				
			}
			
		}
		//printf("Iteration:%d \tMAD: %f\n", iteration, minMad);
		
		points[0] = best;
		/*if (minMad > 1.0 && (best.x != lastBest.x && best.y != lastBest.y)) {
			step = step;
			iteration++;
		}
		else { step = step / 2; iteration = 0; }*/
		step = step / 2;
		

	}
	//printf("MinMAD: %f\n", minMad);
	///////// vracanje na gornji lijevi ugao makrobloka
	best.x -= (blockSize / 2);
	best.y -= (blockSize / 2);
	if (minMad > 3.00) {
		best = macroblockCoords;
	}
	
	
	return best;
}
Point* getSearchArea(Point macroblockCoords, int searchArea, int width, int height,int blockSize) {
	Point* pointsSA;
	pointsSA = malloc(2 * sizeof(Point));
	int x = macroblockCoords.x  - searchArea;
	if (x < 0) { x = 0; }
	
	int y = macroblockCoords.y  - searchArea;
	if (y < 0) { y = 0; }
	
	Point point;
	pointsSA[0].x = x;
	pointsSA[0].y = y;
	x = macroblockCoords.x + blockSize + searchArea;
	if (x > width) { x = width-blockSize; }
	y = macroblockCoords.y + blockSize + searchArea;
	if (y > height) { y = height-blockSize; }
	pointsSA[1].x = x;
	pointsSA[1].y = y;
	return pointsSA;
}
Point getBestMatchEBMA(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize,int searchArea) {
	Point best, priv; 
	Point* pointsSA = getSearchArea(macroblockCoords, searchArea, WIDTH, HEIGHT,blockSize);
	Point currentPoint;
	
	uint8_t currentMacroblockBuffer[BLOCK_ELEMENTS];/////blockSize*blockSize
	
	long float minMad = 99999.999;
	long float MAD;
	best = macroblockCoords;
	int nums = 0;
	currentPoint.x = macroblockCoords.x + (blockSize / 2);
	currentPoint.y = macroblockCoords.y + (blockSize / 2);
	getBlock(frameCurrent, WIDTH, HEIGHT, blockSize, currentPoint, currentMacroblockBuffer);
	MAD = calculateMAD(refMacroblock, currentMacroblockBuffer, blockSize);
	if (MAD < minMad) {
		minMad = MAD;
		best = currentPoint;
	}
	for (int y = pointsSA[0].y; y < pointsSA[1].y; y++) {
		for (int x = pointsSA[0].x; x < pointsSA[1].x; x++) {
			currentPoint.x = x + (blockSize / 2);
			currentPoint.y = y + (blockSize / 2);
			getBlock(frameCurrent, WIDTH, HEIGHT, blockSize, currentPoint, currentMacroblockBuffer);
			MAD = calculateMAD(refMacroblock, currentMacroblockBuffer, blockSize);
			
			if (MAD < minMad) {
				minMad = MAD;
				best = currentPoint;
			}
		}

	}
	//printf("MinMAD EBMA: %f\n",minMad);
	free(pointsSA);
		

	///////// vracanje na gornji lijevi ugao makrobloka
	best.x -= (blockSize / 2);
	best.y -= (blockSize / 2);

	return best;
}

Point* blockMatchingTSS(Vector* matches,uint8_t* currentFrame, uint8_t* prevFrame, int blockSize, int step) {
	uint8_t* bufferMacroblock = malloc(BLOCK_ELEMENTS);
	int macroblocksCount = (WIDTH / blockSize) * (HEIGHT / blockSize);
	Point point;
	Point best;///debug varijabla 
	int deviation;
	int index = 0;
	for (int y = 0; y < HEIGHT-(HEIGHT%BLOCK_SIZE); y += blockSize) {
		for (int x = 0; x <WIDTH-(WIDTH%BLOCK_SIZE); x +=blockSize) {
			point.x = x+((blockSize / 2));
			point.y = y+ ((blockSize / 2));
			getBlock(prevFrame, WIDTH, HEIGHT, blockSize, point, bufferMacroblock);
			point.x = x;
			point.y = y;
			matches[index].from = point;
			deviation = blockValueDeviation(bufferMacroblock, blockSize);
			if (deviation>500) {
				//printf("Deviation: %d\n", deviation);
				matches[index].to = getBestMatchTSS(point, bufferMacroblock, currentFrame, blockSize, step);
				
			}
			else {
				matches[index].to = point;
			}
			
			int result = atan2(matches[index].to.y - matches[index].from.y, matches[index].to.x - matches[index].from.x) * 180 / PI;
			matches[index].angle = calculateAngle2Points(matches[index].to, matches[index].from);
			matches[index].length = calculateLength2Points(matches[index].from, matches[index].to);
			//printf("(%d,%d)", matches[index].x, matches[index].y);
			
			index++;
			
			
		}
	}
	
	free(bufferMacroblock);
	return matches;
}

Point* blockMatchingEBMA(Vector* matches, uint8_t* currentFrame, uint8_t* prevFrame, int blockSize,int searchArea) {
	uint8_t* bufferMacroblock = malloc(BLOCK_ELEMENTS);
	int macroblocksCount = (WIDTH / blockSize) * (HEIGHT / blockSize);
	Point point;
	Point best;///debug varijabla 
	int deviation;
	int index = 0;
	for (int y = 0; y < HEIGHT - (HEIGHT % BLOCK_SIZE); y += blockSize) {
		for (int x = 0; x < WIDTH - (WIDTH % BLOCK_SIZE); x += blockSize) {
			point.x = x + ((blockSize / 2));
			point.y = y + ((blockSize / 2));
			getBlock(prevFrame, WIDTH, HEIGHT, blockSize, point, bufferMacroblock);
			
			point.x = x;
			point.y = y;
			
			matches[index].from = point;
			deviation = blockValueDeviation(bufferMacroblock, blockSize);
			if (deviation >500 ) {
				//printf("Deviation: %d\n", deviation);
				matches[index].to = getBestMatchEBMA(point, bufferMacroblock, currentFrame, blockSize,searchArea);

			}
			else {
				matches[index].to = point;
			}
			


			int result = atan2(matches[index].to.y - matches[index].from.y, matches[index].to.x - matches[index].from.x) * 180 / PI;
			matches[index].angle = calculateAngle2Points(matches[index].to,matches[index].from);
			matches[index].length = calculateLength2Points(matches[index].from, matches[index].to);
			//printf("(%d,%d)", matches[index].x, matches[index].y);
			index++;


		}
	}

	free(bufferMacroblock);
	return matches;
}
Point* blockMatchingDSP(Vector* matches, uint8_t* currentFrame, uint8_t* prevFrame) {
	uint8_t* bufferMacroblock = malloc(BLOCK_ELEMENTS);
	int macroblocksCount = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	Point center;
	Point best;///debug varijabla 
	int deviation;
	int index = 0;
	for (int y = 0; y < HEIGHT; y += BLOCK_SIZE) {
		for (int x = 0; x < WIDTH; x += BLOCK_SIZE) {
			Point upperLeft = { x,y };
			center = getCenter(upperLeft);

			Point priv = getBlock(prevFrame, WIDTH, HEIGHT, BLOCK_SIZE, center, bufferMacroblock);
			center = priv;
			matches[index].from = upperLeft;
			deviation = blockValueDeviation(bufferMacroblock, BLOCK_SIZE);
			if (deviation > 500) {
				//printf("Deviation: %d\n", deviation);
				priv =  searcherLDSP(center, bufferMacroblock, currentFrame, 999999.9);
				matches[index].to = getUpperLeft(priv);
				//printf("From: (%d,%d), To: (%d,%d)\n", matches[index].from.x, matches[index].from.y, matches[index].to.x, matches[index].to.y);

			}
			else {
				matches[index].to = upperLeft;
			}

			int result = atan2(matches[index].to.y - matches[index].from.y, matches[index].to.x - matches[index].from.x) * 180 / PI;
			matches[index].angle = calculateAngle2Points(matches[index].to, matches[index].from);
			matches[index].length = calculateLength2Points(matches[index].from, matches[index].to);
			

			index++;


		}
	}

	free(bufferMacroblock);
	return matches;
}

void findMinMaxColumns(int* minima, int* maxima, int ftr_num, int** vectorsFiltered, int countOfFilteredVectors) {
	int32_t index = 0;
	
	while (index < ftr_num) {
		minima[index] = INT_MAX;
		maxima[index] = INT_MIN;
		index++;
	}

	int ftr_i = 0;
	for (index = 0; index < countOfFilteredVectors; index++) {
		for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
			if (vectorsFiltered[index][ftr_i] < minima[ftr_i]) {
				minima[ftr_i] = vectorsFiltered[index][ftr_i];
			}
			if (vectorsFiltered[index][ftr_i] > maxima[ftr_i]) {
				maxima[ftr_i] = vectorsFiltered[index][ftr_i];
			}
		}
	}
	
}
int getRandom(int* minima, int* maxima) {
	srand(time(0));
	return RANDOM(((*minima) + 1), ((*maxima) - 1));
}
void initializeMeans(int** means, int k, int ftr_num, int* minima, int* maxima) {
	/*
	means- onoliko koliko je k*ftr_num
	*/
	int i,ftr_i;
	for (i = 0; i < k; i++) {
		for (ftr_i = 0; ftr_i < ftr_num; ftr_i) {
			means[i][ftr_i] = getRandom(&minima[ftr_i], &maxima[ftr_i]);
		}
	}
}

float euclideanDistance(int** vectors,int v_index, int* mean, int ftr_num) {
	float sum = 0;
	int ftr_i;
	for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
		sum += pow(vectors[ftr_i][v_index] - mean[ftr_i],2);
	}
	return sqrt(sum);
}

void updateMean(int clusterSize, int* mean, int** vectors,int v_index, int ftr_num) {
	int ftr_i;
	int newMean;
	for (ftr_i = 0; ftr_i < ftr_num; ftr_i++) {
		newMean = mean[ftr_i];
		newMean = (newMean * (clusterSize - 1) + vectors[ftr_i][v_index]) / clusterSize;
		mean[ftr_i] = newMean;
	}
}

int classify(int** means, int** vectors, int v_index,int k, int ftr_num) {
	int minimum = INT_MAX;
	int index = -1;
	int i;
	float dis;
	for (i = 0; i < (k * ftr_num); i++) {
		dis = euclideanDistance(vectors, v_index, *(means + i), ftr_num);
		if (dis < minimum) {
			minimum = dis;
			index = i;
		}
	}
	return index;

}
////row= 4, columns = 5
//int** items = (int**)malloc(4 * sizeof(int*));
//for (int i = 0; i < 4; i++) {
//	(items[i]) = (int*)malloc(5 * sizeof(int));
//}
//int value = 1;
//for (int i = 0; i < 4; i++) {
//	for (int j = 0; j < 5; j++) {
//		items[i][j] = value;
//		value++;
//	}
//}
//int* first_row = *items;
//int* second_row = *(items + 1);

void calculateMeans(int** means, int k, int ftr_num, int** vectors,int countOfVectors, int maxIterations ) {
	int* minima = malloc(ftr_num * sizeof(int));
	int* maxima = malloc(ftr_num * sizeof(int));
	findMinMaxColumns(minima, maxima, ftr_num, vectors, countOfVectors);
	initializeMeans(means, k, ftr_num, minima, maxima);
	int* clusterSizes = calloc(k, sizeof(int));

	int* belongsTo = calloc(countOfVectors, sizeof(int));
	
	int i,j;
	int index;
	int cSize;
	int noChange = 1;
	for (j = 0; j < maxIterations; j++) {
		noChange = 1;
		for (i = 0; i < countOfVectors; i++) {
			index = classify(means, vectors, i, k, ftr_num);
			clusterSizes[index] += 1;
			cSize = clusterSizes[index];
			updateMean(cSize, *(means + index), vectors, i, ftr_num);
			if (index != belongsTo[i]) {
				noChange = 0;
			}

		}
		if (noChange) {
			break;
		}
	}
	

}

VectorsList* findClusters(int** means, int** vectors,int countOfVectors, int k,int ftr_num) {
	VectorsList* clusters = malloc(k * sizeof(VectorsList));
	int i,j,index;
	for (i = 0; i < k; i++) {
		clusters[i].head = NULL;
		clusters[i].itemsNum = 0;
	}
	for (i = 0; i < countOfVectors; i++) {
		index = classify(means, vectors, i, k, ftr_num);
		VectorItem* vector;
		if (clusters[index].head == NULL) {
			clusters[index].head = malloc(sizeof(VectorItem));
			vector = clusters[index].head;
			vector->next== NULL;
			vector->xfrom = vectors[i][0];
			vector->yfrom = vectors[i][1];
			vector->xto	= vectors[i][2];
			vector->yto = vectors[i][3];
			vector->length = vectors[i][4];
			vector->angle = vectors[i][5];

		}
		else {
			vector = clusters[index].head;
			while (vector->next != NULL) {
				vector = vector->next;
			}
			vector->next = malloc(sizeof(VectorItem));
			vector = vector->next;
			vector->next = NULL;
			vector->xfrom = vectors[i][0];
			vector->yfrom = vectors[i][1];
			vector->xto = vectors[i][2];
			vector->yto = vectors[i][3];
			vector->length = vectors[i][4];
			vector->angle = vectors[i][5];
		}
		
		

	}
	return clusters;
}