#define _CRT_SECURE_NO_WARNINGS
#include "read_write_utils.h"
#include "yuv_convert_utils.h"
#include "tss.h"
#include "my_bma.h"
#include "ebma.h"
#include <time.h>
#include "vectors_filtering.h"
#include "hibrid_bma.h"
//#include "paths.h"
#include "evaluation_proces.h"
#include "bma_flow.h"

#define clip_width_start(X) ((X-25)<0?0:(X-25))
#define clip_width_end(X,Y) ((X+25)>Y?Y:(X+25))

int main(void) {
	char* videopath = "D:\\objectFollowSequence.yuv";//"F:\\odabrani_mirna_centerYUYV.yuv"
	
	char* movingPairPath = "D:\\test0.yuv";
	
	char* savePath = "D:\\objectFollowSequence.yuv";
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444 = (uint8_t*)malloc(3*1280*720);
	char* vectorsPath = "H:\\vektori\\vectors0.bin";
	char* belongsToPath = "H:\\vektori\\belongsTo.bin";
	uint8_t* yuyv = (uint8_t*)malloc(WIDTH * HEIGHT * 2 * sizeof(uint8_t));

	char imagePath[100];
	long pairs[5] = {541L,621L,698L, 958L, 995L };
	FILE* f = fopen(videopath, "rb");
	//FILE* append = fopen(savePath, "ab");
	
	/*
	
	for (int i = 0; i < 5; i++) {
		sprintf(savePath, "D:\\boundedsequence%d.yuv", i);
		fread(yuyv, (1280 * 720 * 2), 1, f);
		appendFrameToYUYVFile(savePath, yuyv, 1280, 720);
		fread(yuyv, (1280 * 720 * 2), 1, f);
		appendFrameToYUYVFile(savePath, yuyv, 1280, 720);
		

	}
	
	fclose(f);
	getchar();
	*/
	

	
	/*
	for (int j = 150; j < 570; j++) {
		index = startIndex;
		for (int i = startIndex; i < endIndex; i += 2) {
			y[(j * WIDTH) + index] = yuyv[((j * WIDTH) + index) * 2];
			index++;
			y[(j * WIDTH) + index] = yuyv[((j * WIDTH) + index) * 2];
			index++;

		}
	}
	saveYComponent(savePath, y, 1280, 720);
	free(y);
	y = malloc(1280 * 720);
	startIndex = clip_width_start(640);
	endIndex = clip_width_end(1280, 1280);
	for (int j = 150; j < 570; j++) {
		index = startIndex;
		for (int i = startIndex; i < endIndex; i += 2) {
			y[(j * WIDTH) + index] = yuyv[((j * WIDTH) + index) * 2];
			index++;
			y[(j * WIDTH) + index] = yuyv[((j * WIDTH) + index) * 2];
			index++;

		}
	}
	appendYComponent(savePath, y, 1280, 720);
	

	
	*/


	/*
	uint8_t* currframey=(uint8_t*)malloc(WIDTH*HEIGHT*2);
	uint8_t* prevframey= (uint8_t*)malloc(WIDTH * HEIGHT * 2);
	uint8_t* image;
	int iteration = 20;*/
	/*for (int i = 0; i < iteration; i+=2) {
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, i);
		sprintf(imagePath, "D:\\Videosekvence\\slike\\tests\\test%d.yuv", i);
		appendFrameToYUYVFile(imagePath, image, WIDTH, HEIGHT);
		free(image);
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, i+1);
		appendFrameToYUYVFile(imagePath, image, WIDTH, HEIGHT);
		free(image);

	}*/
	

	
	//getYComponent_YUV422_YUYV(currframey, image, WIDTH, HEIGHT);
	
	//free(image);
	int16_t** vectors = malloc(ftr_num * sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		vectors[i] = malloc(1000 * sizeof(int16_t));
	}
	int16_t** filteredVectors = malloc(ftr_num * sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		filteredVectors[i] = malloc(1000 * sizeof(int16_t));
	}
	
	ftr_num = 1;
	
	uint8_t* belongsTo= (uint8_t*)malloc(1000 * sizeof(uint8_t));
	int numOfVs = bmaFlow(7, vectors,filteredVectors,belongsTo);
	ftr_num = 6;
	
	
	
	


	/*
	vectors = filterByLength(vectors, &numofmatches, ftr_num);
	uint8_t* belongsTo = filterVectorsFlowMoving(vectors, &numofmatches);*/

	saveBelongsTo(belongsToPath, belongsTo, numOfVs);
	saveVectors(vectorsPath, filteredVectors, numOfVs, ftr_num);
	
	//vectors = filterByLength(vectors, &numofmatches, ftr_num);
	/*
	uint8_t* belongsTo = filterVectorsFlowSimple(vectors, &numofmatches);
	
	
	saveBelongsTo(belongsToPath,belongsTo,numofmatches);*/
	//evaluate(dataFolder, resultsFolder, vectors);
	//saveVectors(vectorsPa, vectors, numofmatches, ftr_num);
	
	return 0;
	
}

