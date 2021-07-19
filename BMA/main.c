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
	char* videopath = "C:\\Videosekvence\\dummy_sekvenca.yuv";//"F:\\odabrani_mirna_centerYUYV.yuv"
	
	char* movingPairPath = "D:\\test0.yuv";
	
	char savePath[100];
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444 = (uint8_t*)malloc(3*1280*720);
	char* vectorsPath = "C:\\Dummy evaluacija\\1280x720\\Vektori\\";
	char* belongsToPath = "C:\\Dummy evaluacija\\BelongsTo\\";
	int steps[3] = { 15,25,35 };
	char directions[8][15] = { "Right","Down","DownRight","Left","Up","UpLeft","UpRight","DownLeft" };
	char objects[3][10] = { "128x72","256x144","512x288" };
	uint8_t* yuyv = (uint8_t*)malloc(WIDTH * HEIGHT * 2 * sizeof(uint8_t));
	uint8_t* prevY = (uint8_t*)malloc(WIDTH * HEIGHT);
	uint8_t* currY = (uint8_t*)malloc(WIDTH * HEIGHT);
	int16_t** vectors = malloc(ftr_num * sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		vectors[i] = malloc(14400 * sizeof(int16_t));
	}
	int16_t** filteredVectors = malloc(ftr_num * sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		filteredVectors[i] = malloc(1000 * sizeof(int16_t));
	}
	char imagePath[100];
	volatile int numOfVs=0;
	FILE* f = fopen(videopath, "rb");
	for (int j = 0; j < 3; j++) {
		for (int z = 0; z < 3; z++) {
			for (int i = 0; i < 8; i++) {
				fread(yuyv, (WIDTH * HEIGHT * 2), 1, f);
				getYComponent_YUV422_YUYV(prevY, yuyv, WIDTH, HEIGHT);
				fread(yuyv, (WIDTH * HEIGHT * 2), 1, f);
				getYComponent_YUV422_YUYV(currY, yuyv, WIDTH, HEIGHT);
				/*numOfVs = blockMatchingEBMA(vectors, currY, prevY, 15, (Point) { .x = 0, .y = 0 }, (Point) { .x = WIDTH, .y = HEIGHT });

				filterByLength(vectors, filteredVectors, &numOfVs, 6);
				printf("num of vs: %d", numOfVs);
				sprintf(savePath, "%sEBMA_%s_%s_Step=%d.bin", vectorsPath, objects[j], directions[i],steps[z]);
				printf("Save path %s", savePath);
				saveVectors(savePath, filteredVectors, numOfVs, 6);
				numOfVs = blockMatchingMYBMA(vectors, currY, prevY, 4, (Point) { .x = 0, .y = 0 }, (Point) { .x = WIDTH, .y = HEIGHT });
				filterByLength(vectors, filteredVectors, &numOfVs, 6);
				sprintf(savePath, "%sMYBMA_%s_%s_Step=%d.bin", vectorsPath, objects[j], directions[i], steps[z]);
				saveVectors(savePath, filteredVectors, numOfVs, 6);*/
				numOfVs = blockMatchingTSS(vectors, currY, prevY, 15, (Point) { .x = 0, .y = 0 }, (Point) { .x = WIDTH, .y = HEIGHT });
				filterByLength(vectors, filteredVectors, &numOfVs, 6);
				sprintf(savePath, "%sTSS_%s_%s_Step=%d.bin", vectorsPath, objects[j], directions[i], steps[z]);
				saveVectors(savePath, filteredVectors, numOfVs, 6);

			}
		}
		
	}
	fclose(f);
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
	
	
	
	
	
	
	
	


	/*
	vectors = filterByLength(vectors, &numofmatches, ftr_num);
	uint8_t* belongsTo = filterVectorsFlowMoving(vectors, &numofmatches);*/

	
	
	//vectors = filterByLength(vectors, &numofmatches, ftr_num);
	/*
	uint8_t* belongsTo = filterVectorsFlowSimple(vectors, &numofmatches);
	
	
	saveBelongsTo(belongsToPath,belongsTo,numofmatches);*/
	//evaluate(dataFolder, resultsFolder, vectors);
	//saveVectors(vectorsPa, vectors, numofmatches, ftr_num);
	
	return 0;
	
}

