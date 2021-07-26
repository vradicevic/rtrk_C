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
	char* vectorsPath = "D:\\vektori\\vectors.bin";
	char* belongsToPath = "D:\\vektori\\belongsTo.bin";

	char* logPath = "D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\";

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
	Point start = (Point) { .x = 0, .y = 200 };
	Point end = (Point) { .x = 1280, .y = 520 };
	int EoF;
	uint8_t* image;


	for (int z = 0; z < 40; z += 2) {
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, z);
		getYComponent_YUV422_YUYV(prevY, image, WIDTH, HEIGHT);
		free(image);
		image = readFrameFrom422YUYVVideo(videopath, WIDTH, HEIGHT, z + 1);
		getYComponent_YUV422_YUYV(currY, image, WIDTH, HEIGHT);
		free(image);
		numOfVs = blockMatchingEBMA(vectors, currY, prevY, 25, start, end);
		filterByLength(vectors, filteredVectors, &numOfVs, 6);

		belongsTo = filterNewMethod1(filteredVectors, &numOfVs);

		saveBelongsTo(belongsToPath, belongsTo, numOfVs);
		saveVectors(vectorsPath, filteredVectors, numOfVs, 6);
		getchar();
	}
	
	
	return 0;
	
}

