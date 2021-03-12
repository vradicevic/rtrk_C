#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 
#include "utils.h"




int main()
{
	char* fileName = "D:\\Visual Studio\\YUV\\captureYUV422P.yuv";
	
	int offsetName[] = {10,20,30,50,100,200,300};
	int8_t blockSize = BLOCK_SIZE;
	int step = 35;// oko 13 najbolje
	int searchArea = 15; // 15 sigurno najbolje
	int steps[] = {15,25,35,45,55,65,75,85,95};
	
	uint32_t frameSize = (WIDTH * HEIGHT * 2); //YUV422 YUYV format
	int countOfMatches = (WIDTH / blockSize) * (HEIGHT / blockSize);

	Vector* matches = malloc(countOfMatches * sizeof(Vector));

	FILE* fp2;

	
	char* WcurrentFrame = "D:\\ProgramskiDokumenti\\VS Code\\Pythonprojects\\rtrk\\images\\FRAME.yuv";
	char* yuyvVideo = "D:\\YUV422YUYV testne slike\\carAway.yuv";
	char* yFrom420Frame = "D:\\ProgramskiDokumenti\\VS Code\\Pythonprojects\\rtrk\\yframefrom420.yuv";
	char* frame1 = "D:\\auticNew\\autic327.yuv";
	char* frame2 = "D:\\auticNew\\autic328.yuv";
	char prevFramePath[100];
	char currentFramePath[100];
	char* vectorsFilePath;
	uint8_t* frameYUYV1 = getFrameFrom422YUYVVideo(yuyvVideo, WIDTH, HEIGHT, 0);
	uint8_t* frameYUYV2 = getFrameFrom422YUYVVideo(yuyvVideo, WIDTH, HEIGHT, 1);

	


	
	uint8_t* prevBufferY;
	uint8_t* currentBufferY ;
	
	/*prevBufferY = getYComponent_YUV444_YUV(frame1, WIDTH, HEIGHT);

	currentBufferY = getYComponent_YUV444_YUV(frame2, WIDTH, HEIGHT);*/
	prevBufferY = getYComponent_YUV422_YUYV(frameYUYV1, frameSize, WIDTH, HEIGHT);
	currentBufferY = getYComponent_YUV422_YUYV(frameYUYV2, frameSize, WIDTH, HEIGHT);


	//TSS

	
	blockMatchingEBMA(matches, currentBufferY, prevBufferY,BLOCK_SIZE,searchArea);
	free(prevBufferY);
	free(currentBufferY);
	vectorsFilePath = "D:\\Visual Studio Code\\rtrk\\vectors\\carAwayEBMAblock32.bin";

	
	fp2 = fopen(vectorsFilePath, "wb");
	fwrite(matches, sizeof(Vector), countOfMatches, fp2);
	fclose(fp2);
	
	
	
	
	

	
	//EBMA
	/*blockMatchingEBMA(matches, currentBufferY, prevBufferY, blockSize,searchArea);

	vectorsFilePath = "D:\\Visual Studio Code\\rtrk\\vectors\\carawayblackBlock32Step15EBMA.bin";
	fp2 = fopen(vectorsFilePath, "wb");
	fwrite(matches, sizeof(Vector), countOfMatches, fp2);
	fclose(fp2);*/
	




	
	
	
	

	
	
	
	
	//blockMatchingEBMA(matches, currentBufferY, prevBufferY, blockSize,searchArea);
	//blockMatchingTSS(matches, currentBufferY, prevBufferY, blockSize, step);
	
	
	
	//Filtriranje vektora
	
	//filterVectorsByOrientation(matches, blockSize);
	//filterVectorsForDuplicationError(matches, WIDTH, HEIGHT, blockSize);
	//fp2 = fopen(vectorsFilePath, "wb");
	//fwrite(matches, sizeof(Vector), countOfMatches, fp2);
	//fclose(fp2);
	//free(prevBufferY);
	//free(currentBufferY);
	///////////////////////////////////////////////////////////////////////////////////////////
	free(matches);
	
	
	



	

	return 0;
	

}
