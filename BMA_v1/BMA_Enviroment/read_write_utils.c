#include "read_write_utils.h"
uint8_t* readFileYUV422_YUYV(char* filepath,int width, int height) {
    uint8_t* yuyv = (uint8_t*) malloc(width*height*2*sizeof(uint8_t));
    FILE* file = fopen(filepath, "rb");
    fread(yuyv, 1, (width*height*2), file);
    fclose(file);
    return yuyv;
}
uint8_t* readFileYUV420P_YpUVp(char* filepath, int width, int height) {
    uint8_t* yuv420P = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    FILE* file = fopen(filepath, "rb");
    fread(yuv420P, 1, (width * height), file);
    fclose(file);
    return yuv420P;
}

uint8_t* readFileYUV444_YUV(char* filepath, int width, int height) {
    uint8_t* yuv444 = (uint8_t*)malloc(width * height *3* sizeof(uint8_t));
    FILE* file = fopen(filepath, "rb");
    fread(yuv444, 1, width * height * 3, file);
    fclose(file);
    return yuv444;
}


void getYComponent_YUV422_YUYV(uint8_t* Y, uint8_t* image, int width, int height) {
    int index = 0;
    int flag = 1;
    int ys = 0;
    int frameSize=width*height*2;

    while (index < frameSize) {
        if (flag == 1) {
            Y[ys] = image[index];
            ys++;
            flag = 0;
        }
        else {
            flag = 1;
        }
        index++;
    }

}

void getYComponent_YUV420P_YpUVp(uint8_t* Y, uint8_t* image, int width, int height) {
    int i;
    for (i = 0; i < (width * height); i++) {
        Y[i] = image[i];
    }
}

void getYComponent_YUV444_YUV(uint8_t* Y, uint8_t* image, int width, int height) {
    int i,j=0;
    for (i = 0; i < (width * height*3); i=i+3) {
        Y[j++] = image[i];
    }
}

uint8_t* readFrameFrom422YUYVVideo(char* filepath, int width, int height, int frameNumber) {//prvi frame ima frameNumber = 0
    int frameSize = (width * height * 2);
    uint8_t* yuyv = (uint8_t*)malloc(frameSize*sizeof(uint8_t));
    FILE* file = fopen(filepath, "rb");
    fseek(file, frameNumber * frameSize, 0);
    fread(yuyv, 1, frameSize, file);
    fclose(file);
    return yuyv;

}

uint8_t* readFrameFrom444YUVVideo(char* filepath, int width, int height, int frameNumber) {//prvi frame ima frameNumber = 1
    int frameSize = (width * height * 3);
	frameNumber = frameNumber - 1;
    uint8_t* yuv444 = (uint8_t*)malloc(frameSize * sizeof(uint8_t));
    FILE* file = fopen(filepath, "rb");
    fseek(file, frameNumber * frameSize,SEEK_SET);
    fread(yuv444, sizeof(uint8_t), frameSize, file);
    fclose(file);
    return yuv444;
}



void saveVectors(char* filepath, int16_t** buff, int16_t itemsNum, int wordWidth) {//u slucaju vektora itemsNum je num of vectors, a wordWidth je ftr_num
    FILE* file = fopen(filepath, "wb");
    //fwrite(&itemsNum, sizeof(int16_t), 1, file);
    
    for (int i = 0; i < itemsNum; i++) {
        for (int j = 0; j < wordWidth; j++) {
            
            fwrite(&buff[j][i], sizeof(int16_t), 1, file);
        }
    }
    fclose(file);
    printf("File write done\n");
}

void appendFrameToYUYVFile(char* filepath, uint8_t* yuyv, int width, int height) {
	FILE* file = fopen(filepath, "ab");
	fwrite(yuyv, sizeof(uint8_t), (width * height * 2), file);
	fclose(file);
}
void appendFrameToYUV444File(char* filepath, uint8_t* yuv444, int width, int height) {
	FILE* file = fopen(filepath, "ab");
	fwrite(yuv444, sizeof(uint8_t), (width * height * 3), file);
	fclose(file);
}

void saveYUYVImage(char* imagePath, uint8_t image, int width, int height) {
    FILE* file = fopen(imagePath, "wb");
    fwrite(image, sizeof(uint8_t), (width * height * 2), file);
    fclose(file);
}

void saveBelongsTo(char* filepath, uint8_t* buff, int itemsNum) {
	FILE* file = fopen(filepath, "wb");
	//fwrite(&itemsNum, sizeof(int16_t), 1, file);
	fwrite(buff, sizeof(uint8_t), itemsNum, file);
	fclose(file);
	printf("Write belongsTo done\n");
}

void saveYComponent(char* filepath,uint8_t* buffY,int width, int height) {
    FILE* file = fopen(filepath, "wb");
    fwrite(buffY, sizeof(uint8_t), width * height, file);
    fclose(file);
    printf("\nSave Y done!");
}
void appendYComponent(char* filepath, uint8_t* buffY, int width, int height) {
    FILE* file = fopen(filepath, "ab");
    fwrite(buffY, sizeof(uint8_t), width * height, file);
    fclose(file);
    printf("\nSave Y done!");
}