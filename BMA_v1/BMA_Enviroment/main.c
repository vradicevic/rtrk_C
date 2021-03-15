#define _CRT_SECURE_NO_WARNINGS
#include "read_write_utils.h"
#include "tss.h"
#include "my_bma.h"


int main(void) {
	char* videoPath = "H:\\VISION_SDK_02_12_01_00\\vision_sdk\\tools\\network_tools\\bin\\YUV422YUYV_sekvence\\auticNew.yuv";
	char* vectorsPath = "H:\\vektori\\vectors0.bin";
	int ftr_num = 6;
	int maxMatches = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	uint8_t* prevFrameY = (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	uint8_t* currFrameY = (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));

	int16_t** vectors = (int16_t**)malloc(sizeof(int16_t*)*ftr_num);
	for (int i = 0; i < ftr_num; i++) {
		vectors[i] = (int16_t*)malloc(maxMatches * sizeof(int16_t));
	}

	uint8_t* image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT, 0);
	getYComponent_YUV422_YUYV(prevFrameY, image, WIDTH, HEIGHT);
	free(image);

	image = readFrameFrom422YUYVVideo(videoPath, WIDTH, HEIGHT, 1);
	getYComponent_YUV422_YUYV(currFrameY, image, WIDTH, HEIGHT);
	free(image);
	

	int numOfMatches = blockMatchingMYBMA(vectors,currFrameY,prevFrameY,15);
	saveVectors(vectorsPath, vectors, numOfMatches, ftr_num);

	return 1;
}