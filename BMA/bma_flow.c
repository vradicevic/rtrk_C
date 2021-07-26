#include "bma_flow.h"
#include "paths.h"
#include "vectors_filtering.h"
#include "read_write_utils.h"

uint8_t* bmaFlow(int step, int16_t** vectors, int16_t** filteredVectors, int* numOfVs) {
	Point start, end;
	
	start.x = 0;
	end.x = WIDTH;
	start.y = 200;
	end.y = HEIGHT-200;
	int ftr_num = 6;
	int maxMatches = (HEIGHT / BLOCK_SIZE) * (WIDTH / BLOCK_SIZE);
	uint8_t* image;
	uint8_t* prevFrame = (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	uint8_t* currFrame= (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	char* videoPath = "D:\\nesto";

	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT,22);

	getYComponent_YUV422_YUYV(prevFrame, image, WIDTH, HEIGHT);

	free(image);
	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT, 23);
	getYComponent_YUV422_YUYV(currFrame, image, WIDTH, HEIGHT);
	free(image);
	
	*numOfVs = blockMatchingMYBMA(vectors, currFrame, prevFrame, step, start, end);
	filterByLength(vectors,filteredVectors, numOfVs, ftr_num);
	uint8_t* belongsTo = filterVectorsBackSub(vectors, numOfVs);
	//groupVectors(filteredVectors, belongsTo,numMatches, 10);
	
	return belongsTo;
}


void mainDummyEval(void) {
	char* videopath = "C:\\Videosekvence\\dummy_sekvenca.yuv";//"F:\\odabrani_mirna_centerYUYV.yuv"

	char* movingPairPath = "D:\\test0.yuv";

	char savePath[100];
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444 = (uint8_t*)malloc(3 * 1280 * 720);
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
	volatile int numOfVs = 0;
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
}


void mainTimeTest(void) {
	clock_t startTime, endTime;
	int cpuTime;

	char* videopath = "D:\\Videosekvence\\yuv\\odabrani final\\finalfinal\\dummy_sekvenca.yuv";

	int frameSize = WIDTH * HEIGHT * 2;

	char savePath[100];
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444 = (uint8_t*)malloc(3 * 1280 * 720);
	char* vectorsPath = "D:\\Videosekvence\\Dummy evaluacija\\1280x720\\Vektori\\";
	char* belongsToPath = "C:\\Dummy evaluacija\\BelongsTo\\";

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
	volatile int numOfVs = 0;
	Point start = (Point) { .x = 0, .y = 200 };
	Point end = (Point) { .x = 1280, .y = 520 };

	FILE* f = fopen(videopath, "rb");


	int EoF;


	//neslijedni
	for (int i = 0; i < 72; i++) {
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
		appendLog("D:\\logovi\\PC\\1280x720\\OPTI\\EBMA_dummy_block=32TEST.log", log);
		/*
		startTime = clock();
		blockMatchingTSS(vectors, currY, prevY, 7, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\TSS_dummy_block=16.log", log);

		startTime = clock();
		blockMatchingMYBMA(vectors, currY, prevY, 4, start, end);
		endTime = clock();

		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\MYBMA_dummy_block=16.log", log);

		*/

	}



	/*
	uint8_t* temp;
	//slijedni
	fread(yuyv, 1, frameSize, f);
	getYComponent_YUV422_YUYV(prevY, yuyv, WIDTH, HEIGHT);
	for (int i = 0; i < 39; i++) {
		startTime = clock();
		EoF = fread(yuyv, 1, frameSize, f);
		if (EoF == 0) break;
		getYComponent_YUV422_YUYV(currY, yuyv, WIDTH, HEIGHT);


		blockMatchingEBMA(vectors, currY, prevY, 15, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\EBMA_slijedniKamion_block=16.log", log);

		startTime = clock();
		blockMatchingTSS(vectors, currY, prevY, 7, start, end);
		endTime = clock();
		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\TSS_slijedniKamion_block=16.log", log);

		startTime = clock();
		blockMatchingMYBMA(vectors, currY, prevY, 4, start, end);
		endTime = clock();

		cpuTime = (int)((((double)(endTime - startTime)) / CLOCKS_PER_SEC) * 1000);
		sprintf(log, "[DSP1  ] BMA:Frame=%d  Time=%d\n", i, cpuTime);
		appendLog("D:\\logovi\\PC\\640x360\\Block=16\\OPTI\\MYBMA_slijedniKamion_block=16.log", log);

		temp = prevY;
		prevY = currY;
		currY = temp;

	}

	*/










}