#include "yuv_convert_utils.h"
void convertYUV444ToYUYV422(uint8_t* yuv444, uint8_t* yuyv, int width, int height) {
	int i, j;
	int frameSize = width * height * 2;
	int counter = 0;
	for (i = 0; i < frameSize; i = i + 4)
	{
		yuyv[i] = yuv444[counter];
		yuyv[i + 1] = yuv444[counter + 1];
		yuyv[i + 2] = yuv444[counter + 3];
		yuyv[i + 3] = yuv444[counter + 5];
		counter = counter + 6;
	}
}