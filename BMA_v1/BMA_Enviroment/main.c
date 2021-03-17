#define _CRT_SECURE_NO_WARNINGS
#include "read_write_utils.h"
#include "yuv_convert_utils.h"
#include "tss.h"
#include "my_bma.h"


int main(void) {
	char* videoPath = "D:\\Videosekvence\\yuv\\testingNew.yuv";
	char* outputYUYVPath = "D:\\Videosekvence\\yuv\\odabrani_mirna_rightYUYV.yuv";
	char* testYUV444 = "D:\\Videosekvence\\yuv\\tester444.yuv";
	char* vectorsPath = "H:\\vektori\\vectors0.bin";
	int frames[] = {51,53,55,63,65,67,75,77,83,87,91,147,151,155,165,187,189,191,195,199,207,215,223,233,239,249,261,267,325,329,335,345,355,357,361};
	int framesNum = 35;
	int ftr_num = 6;
	
	int maxMatches = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	uint8_t* yuv444;
	uint8_t* yuyv = (uint8_t*)malloc(WIDTH * HEIGHT*2 * sizeof(uint8_t));
	
	
	for (int i = 0; i < framesNum; i++) {
		yuv444 = readFrameFrom444YUVVideo(videoPath, WIDTH, HEIGHT, frames[i]);
		convertYUV444ToYUYV422(yuv444, yuyv, WIDTH, HEIGHT);
		appendFrameToYUYVFile(outputYUYVPath, yuyv, WIDTH, HEIGHT);
		free(yuv444);

		yuv444 = readFrameFrom444YUVVideo(videoPath, WIDTH, HEIGHT, frames[i] +1);
		convertYUV444ToYUYV422(yuv444, yuyv, WIDTH, HEIGHT);
		appendFrameToYUYVFile(outputYUYVPath, yuyv, WIDTH, HEIGHT);
		free(yuv444);
		
	}
	printf("program finished");













	/*int16_t** vectors = (int16_t**)malloc(sizeof(int16_t*)*ftr_num);
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
*/
	return 1;
}