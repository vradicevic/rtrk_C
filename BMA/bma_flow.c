#include "bma_flow.h"
#include "paths.h"

int bmaFlow(int step, int16_t** vectors) {
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
	char* videoPath = videoMirnaCenter30FPS;
	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT,12);
	getYComponent_YUV422_YUYV(prevFrame, image, WIDTH, HEIGHT);
	free(image);
	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT, 13);
	getYComponent_YUV422_YUYV(currFrame, image, WIDTH, HEIGHT);
	 
	
	int numMatches = blockMatchingMYBMA(vectors, currFrame, prevFrame, step, start, end);
	
	printf("BlockMatching done!\n");
	return numMatches;
}