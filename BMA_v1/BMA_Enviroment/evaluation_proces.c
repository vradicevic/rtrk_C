#include "evaluation_proces.h"

void evaluate(char* dataFolderPath,char* resultsFolderPath,int16_t** vectors) {
	char dataPath[120] = "\\Offset15Frame0.yuv";
	char resultsPath[120];
	
	sprintf(dataPath, dataFolderPath);
	
	sprintf(dataPath + strlen(dataPath), "\\Offset15Frame0.yuv");
	uint8_t* yuv = readFileYUV444_YUV(dataPath, 1280, 720);
	uint8_t* yPrev = (uint8_t*)malloc(1280 * 720* sizeof(uint8_t));
	uint8_t* yCurr = (uint8_t*)malloc(1280 * 720*  sizeof(uint8_t));
	int numOfMatches = 0;
	getYComponent_YUV444_YUV(yPrev, yuv, 1280, 720);
	free(yuv);
	for(int offset = 15;offset<46;offset+=10){
		for (int frame = 1; frame < 9; frame++) {
			sprintf(dataPath, dataFolderPath);
			sprintf(resultsPath, resultsFolderPath);
			sprintf(dataPath + strlen(dataPath), "\\Offset%dFrame%d.yuv", offset, frame);
			sprintf(resultsPath + strlen(resultsPath), "\\vectorsOffset%dFrame%d.bin", offset, frame);
			
			yuv = readFileYUV444_YUV(dataPath, 1280, 720);
			
			getYComponent_YUV444_YUV(yCurr, yuv, 1280, 720);
			free(yuv);
			numOfMatches = blockMatchingEBMA(vectors, yCurr, yPrev, 55);
			vectors = filterByLength(vectors, &numOfMatches, 6);
			saveVectors(resultsPath, vectors, numOfMatches, 6);
		}
	}
	printf("\nEvaluation finished!!\n");



}