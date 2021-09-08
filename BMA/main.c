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

#include <time.h>



int main(void) {
	clock_t startTime, endTime;
	int cpuTime;

	char* videopath = "D:\\Videosekvence\\yuv\\odabrani final\\finalfinal\\mirna.yuv";
	
	int frameSize = WIDTH * HEIGHT*2;
	
	char savePath[100];
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* belongsTo;
	uint8_t* yuv444 = (uint8_t*)malloc(3*1280*720);
	char* vectorsPath = "D:\\vektori\\bb\\vectors\\vectors";
	char* belongsToPath = "D:\\vektori\\belongsTo";
	char* clusterInfoPath = "D:\\vektori\\bb\\clusterInfo\\clusterInfo";
	char outPath[100];

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
		filteredVectors[i] = malloc(14400 * sizeof(int16_t));
	}
	int16_t** interVectors = malloc(ftr_num * sizeof(int16_t*));
	for (int i = 0; i < ftr_num; i++) {
		interVectors[i] = malloc(14400 * sizeof(int16_t));
	}
	char log[100];
	volatile int numOfVs=0;
	Point start = (Point) { .x = 0, .y = 200 };
	Point end = (Point) { .x = 1280, .y = 520 };
	int EoF;
	uint8_t* image;
	/*
	int a = 1;
	for (int z = 0; z < 40; z += 2) {
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, z);
		getYComponent_YUV422_YUYV(prevY, image, WIDTH, HEIGHT);
		free(image);
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, z + 1);
		getYComponent_YUV422_YUYV(currY, image, WIDTH, HEIGHT);
		free(image);
		numOfVs = blockMatchingMYBMA(vectors, currY, prevY, 7, start, end);
		filterByLength(vectors, filteredVectors, &numOfVs, 6);
		startTime = clock();
		printf("\nSlika %d\n", a++);
		belongsTo = filterNewMethod1(filteredVectors, &numOfVs);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		printf("Time of filtering = %d\n", cpuTime);

		sprintf(outPath, "%s%d.bin", belongsToPath, z);
		saveBelongsTo(outPath, belongsTo, numOfVs);
		sprintf(outPath, "%s%d.bin", vectorsPath, z);
		
		saveVectors(outPath, filteredVectors, numOfVs, 6);
		
	}
	*/
	uint8_t* nv12 = malloc(WIDTH * HEIGHT * 1.5);
	FILE* file = fopen("D:\\Videosekvence\\yuv\\finalfinal\\marked\\mirnaNV12.yuv", "rb");
	
	
	Point bb[8];
	int k = 0;
	
	//printf("K=%d; startx=%d; starty=%d; endx=%d; endy=%d\n", kaa, bb[0].x, bb[0].y, bb[1].x, bb[1].y);

	for (int i = 0; i < 20; i++) {
		fread(nv12, sizeof(uint8_t), WIDTH* HEIGHT* 1.5, file);
		k = extractBBInfo(nv12, WIDTH, HEIGHT, bb);
		getYComponent_YUV420P_YpUVp(prevY, nv12, WIDTH, HEIGHT);
		fread(nv12, sizeof(uint8_t), WIDTH* HEIGHT* 1.5, file);
		getYComponent_YUV420P_YpUVp(currY, nv12, WIDTH, HEIGHT);
		int16_t vectorsNum[4];
		int cumm = 0;
		for (int j = 0,num=0; j < k; j++,num+=2) {
			vectorsNum[j] = blockMatchingEBMA(vectors, currY, prevY, 25, bb[num], bb[num + 1]);
			filterByLength(vectors, interVectors, &vectorsNum[j], 6);
			appendVectors(filteredVectors, interVectors, vectorsNum[j], cumm);
			
			cumm += vectorsNum[j];
		}
		sprintf(outPath, "%s%d.bin", vectorsPath, i);
		saveVectors(outPath, filteredVectors, cumm, 6);
		sprintf(outPath, "%s%d.bin", clusterInfoPath, i);
		saveClusterInfo(outPath, k, vectorsNum);
		
		
		
	}
	fclose(file);


	return 0;
	
}

