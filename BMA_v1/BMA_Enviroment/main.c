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



int main(void) {
	char* videopath = "D:\\Videosekvence\\yuv\\moving_dashboardYUV444FPS30.yuv";//"F:\\odabrani_mirna_centerYUYV.yuv"
	
	
	
	char* imagePath = "H:\\image.yuv";
	int frames[] = { 39,67,69,75,79,87,93,95,135,137,149,151,153,155,183,185,187,189,191,203,209,219,301,317,321,329,345,347,351,427,429,431,437,463,689,691 };
	int framesNum = 36;
	int ftr_num = 6;
	uint8_t* yuv444;
	uint8_t* yuyv = (uint8_t*)malloc(WIDTH * HEIGHT * 2 * sizeof(uint8_t));
	/*for (int i = 0; i < framesNum; i++) {
		yuv444 = readFrameFrom444YUVVideo(videopath, WIDTH, HEIGHT, frames[i]);
		convertYUV444ToYUYV422(yuv444, yuyv, WIDTH, HEIGHT);
		appendFrameToYUYVFile(outputYUYVPath, yuyv, WIDTH, HEIGHT);
		free(yuv444);
		yuv444 = readFrameFrom444YUVVideo(videopath, WIDTH, HEIGHT, frames[i]+1);
		convertYUV444ToYUYV422(yuv444, yuyv, WIDTH, HEIGHT);
		appendFrameToYUYVFile(outputYUYVPath, yuyv, WIDTH, HEIGHT);
		free(yuv444);

	}
	printf("Everything finished\n");
	*/
	
	int maxMatches = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	int16_t** vectors = (int16_t**)malloc(sizeof(int16_t*)*ftr_num);
	for (int i = 0; i < ftr_num; i++) {
		vectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}
	uint8_t* currframey=(uint8_t*)malloc(WIDTH*HEIGHT*2);
	uint8_t* prevframey= (uint8_t*)malloc(WIDTH * HEIGHT * 2);
	uint8_t* image = readFrameFrom422YUYVVideo(videoMovingDashboard30FPS , WIDTH, HEIGHT, 10);
	getYComponent_YUV422_YUYV(prevframey, image, WIDTH, HEIGHT);
	free(image);

	image = readFrameFrom422YUYVVideo(videoMovingDashboard30FPS, WIDTH, HEIGHT, 11);
	getYComponent_YUV422_YUYV(currframey, image, WIDTH, HEIGHT);
	free(image);

	int numofmatches = blockMatchingHBMA(vectors,currframey,prevframey,15);
	
	//numofmatches = filterVectorsFlow(vectors, numofmatches);
	
	saveVectors(vectorsPath, vectors, numofmatches, ftr_num);
	
	return 0;
	
}