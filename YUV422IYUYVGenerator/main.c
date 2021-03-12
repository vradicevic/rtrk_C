#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 
#define WIDTH 1280
#define HEIGHT 720
int main() /*pretvaranje originalne slike iz 422 u 444*/
{
	

	FILE* fp1;
	FILE* fp2;
	uint8_t* bufferIn;
	uint8_t* bufferYUYV;
	uint8_t* bufferOut;

	uint8_t* y1;
	uint8_t* y2;
	uint8_t* u;
	uint8_t* v;

	uint32_t frameSize;
	uint32_t frameCount;
	uint32_t bufferSize = 0;

	char* fileToOpen1;
	char* fileToOpen2;
	char* fileToOpen3;
	char* fileToOpen4;
	char* fileToOpen5;

	char* fileToWrite;
	char* fileToWrite2;
	char* fileToWrite3;
	char* fileToWrite4;



	fileToOpen1 = "D:\\cars\\car184.yuv";
	fileToOpen2 = "D:\\cars\\car185.yuv";
	fileToOpen3 = "D:\\auticNew\\autic329.yuv";
	fileToOpen4 = "D:\\auticNew\\autic330.yuv";
	fileToOpen5 = "D:\\auticNew\\autic331.yuv";

	fileToWrite = "D:\\YUV422YUYV_sekvence\\cars184_185.yuv";

	frameSize = (WIDTH * HEIGHT * 2 ); //YUV422 format
	bufferSize = (WIDTH * HEIGHT / 2);
	

	
	

	int width = WIDTH;
	int height = HEIGHT;

	bufferIn=malloc(WIDTH*HEIGHT*3);
	fp1 = fopen(fileToOpen1, "rb");
	fread(bufferIn, 1, width * height * 3, fp1);

	uint8_t * output = malloc(WIDTH*HEIGHT*2);
	
	int counter = 0;
	for (int i = 0; i < width * height * 2; i = i + 4)
	{
		output[i] = bufferIn[counter];
		output[i + 1] = bufferIn[counter + 1];
		output[i + 2] = bufferIn[counter + 3];
		output[i + 3] = bufferIn[counter + 5];
		counter = counter + 6;
	}

	
	fp2 = fopen(fileToWrite, "wb");
	fwrite(output, 1, width * height * 2, fp2);
	fclose(fp1);
	fp1 = fopen(fileToOpen2, "rb");
	fread(bufferIn, 1, width * height * 3, fp1);
	counter = 0;
	for (int i = 0; i < width * height * 2; i = i + 4)
	{
		output[i] = bufferIn[counter];
		output[i + 1] = bufferIn[counter + 1];
		output[i + 2] = bufferIn[counter + 3];
		output[i + 3] = bufferIn[counter + 5];
		counter = counter + 6;
	}
	fwrite(output, 1, width * height * 2, fp2);
	/*fp1 = fopen(fileToOpen3, "rb");
	fread(bufferIn, 1, width * height * 3, fp1);
	counter = 0;
	for (int i = 0; i < width * height * 2; i = i + 4)
	{
		output[i] = bufferIn[counter];
		output[i + 1] = bufferIn[counter + 1];
		output[i + 2] = bufferIn[counter + 3];
		output[i + 3] = bufferIn[counter + 5];
		counter = counter + 6;
	}
	fwrite(output, 1, width * height * 2, fp2);
	fp1 = fopen(fileToOpen4, "rb");
	fread(bufferIn, 1, width * height * 3, fp1);
	counter = 0;
	for (int i = 0; i < width * height * 2; i = i + 4)
	{
		output[i] = bufferIn[counter];
		output[i + 1] = bufferIn[counter + 1];
		output[i + 2] = bufferIn[counter + 3];
		output[i + 3] = bufferIn[counter + 5];
		counter = counter + 6;
	}
	fwrite(output, 1, width * height * 2, fp2);
	fp1 = fopen(fileToOpen5, "rb");
	fread(bufferIn, 1, width* height * 3, fp1);
	counter = 0;
	for (int i = 0; i < width * height * 2; i = i + 4)
	{
		output[i] = bufferIn[counter];
		output[i + 1] = bufferIn[counter + 1];
		output[i + 2] = bufferIn[counter + 3];
		output[i + 3] = bufferIn[counter + 5];
		counter = counter + 6;
	}
	fwrite(output, 1, width* height * 2, fp2);*/


	free(bufferIn);
	free(output);

	printf("Hello World! PROGRAM ENDED!\n");

	return 0;
}