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

	char* videopath = "D:\\Videosekvence\\yuv\\odabrani final\\finalfinal\\slijedniKamion.yuv";
	
	int frameSize = WIDTH * HEIGHT*2;
	
	char savePath[100];
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444 = (uint8_t*)malloc(3*1280*720);
	char* vectorsPath = "D:\\Videosekvence\\Dummy evaluacija\\1280x720\\Vektori\\";
	char* belongsToPath = "C:\\Dummy evaluacija\\BelongsTo\\";

	char* logPath = "D:\\logovi\\PC\\1280x720\\NOOPTI\\";

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
	char log[100];
	volatile int numOfVs=0;
	Point start = (Point) { .x = 0, .y = 0 };
	Point end = (Point) { .x = 1280, .y = 720 };

	FILE* f = fopen(videopath, "rb");
	
	
	int EoF;

	/*	
	//neslijedni
	for (int i = 0; i < 20; i++) {
		startTime = clock();
		EoF = fread(yuyv, 1, frameSize, f);
		if (EoF == 0) break;
		getYComponent_YUV422_YUYV(prevY, yuyv, WIDTH, HEIGHT);
		fread(yuyv, 1, frameSize, f);
		getYComponent_YUV422_YUYV(currY, yuyv, WIDTH, HEIGHT);

		blockMatchingEBMA(vectors, currY, prevY, 25, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\EBMA_movingDashboard_block=32.log", log);
		
		startTime = clock();
		blockMatchingTSS(vectors, currY, prevY, 7, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\TSS_dummy_block=32.log", log);

		startTime = clock();
		blockMatchingMYBMA(vectors, currY, prevY, 4, start, end);
		endTime = clock();

		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\MYBMA_dummy_block=32.log", log);
		
		

	}
	*/
	

	
	uint8_t* temp;
	//slijedni
	fread(yuyv, 1, frameSize, f);
	getYComponent_YUV422_YUYV(prevY, yuyv, WIDTH, HEIGHT);
	for (int i = 0; i < 39; i++) {
		startTime = clock();
		EoF = fread(yuyv, 1, frameSize, f);
		if (EoF == 0) break;
		getYComponent_YUV422_YUYV(currY, yuyv, WIDTH, HEIGHT);
		printf("Framsz: %d", EoF / frameSize);

		blockMatchingEBMA(vectors, currY, prevY, 25, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\EBMA_slijedniKamion_block=32.log", log);
		/*
		startTime = clock();
		blockMatchingTSS(vectors, currY, prevY, 7, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\TSS_slijedniKamion_block=32.log", log);

		startTime = clock();
		blockMatchingMYBMA(vectors, currY, prevY, 4, start, end);
		endTime = clock();

		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\1280x720\\NOOPTI\\MYBMA_slijedniKamion_block=32.log", log);
		*/
		temp = prevY;
		prevY = currY;
		currY = temp;

	}
	
	
	
	
	
	
	
	


	
	
	return 0;
	
}

