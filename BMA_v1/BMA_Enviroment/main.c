#define _CRT_SECURE_NO_WARNINGS
#include "read_write_utils.h"
#include "yuv_convert_utils.h"
#include "tss.h"
#include "my_bma.h"
#include "ebma.h"
#include <time.h>
#include "vectors_filtering.h"
#include "hibrid_bma.h"
#include "paths.h"
#include "evaluation_proces.h"



int main(void) {
	char* videopath = "D:\\Videosekvence\\yuv\\moving_dashboardYUV444FPS30.yuv";//"F:\\odabrani_mirna_centerYUYV.yuv"
	
	
	
	char* savePath = "C:\\Videosekvence\\yuv\\moving_selected_five.yuv";
	int frames[] = { 1,5,7,11,19,25 };
	int framesNum = 5;
	int ftr_num = 6;
	uint8_t* yuv444;
	uint8_t* yuyv = (uint8_t*)malloc(WIDTH * HEIGHT * 2 * sizeof(uint8_t));

	

	
	
	int maxMatches = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	int16_t** vectors = (int16_t**)malloc(sizeof(int16_t*)*ftr_num);
	for (int i = 0; i < ftr_num; i++) {
		vectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	uint8_t* currframey=(uint8_t*)malloc(WIDTH*HEIGHT*2);
	uint8_t* prevframey= (uint8_t*)malloc(WIDTH * HEIGHT * 2);
	uint8_t* image = readFrameFrom422YUYVVideo(videoMovingDashboard30FPS, WIDTH, HEIGHT, 10);
	getYComponent_YUV422_YUYV(prevframey, image, WIDTH, HEIGHT);
	
	free(image);

	image = readFrameFrom422YUYVVideo(videoMovingDashboard30FPS, WIDTH, HEIGHT, 11);
	getYComponent_YUV422_YUYV(currframey, image, WIDTH, HEIGHT);
	
	free(image);

	/*int numofmatches = blockMatchingEBMA(vectors,currframey,prevframey,25);
	
	
	
	
	vectors = filterByLength(vectors, &numofmatches, ftr_num);
	
	uint8_t* belongsTo = filterVectorsFlowSimple(vectors, &numofmatches);
	
	saveVectors(vectorsPath, vectors, numofmatches, ftr_num);
	saveBelongsTo(belongsToPath,belongsTo,numofmatches);*/
	evaluate(dataFolder, resultsFolder, vectors);
	
	return 0;
	
}

