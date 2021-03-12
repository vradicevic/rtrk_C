#ifndef _UTILS_H_
#define _UTILS_H_



typedef struct {
	int x;
	int y;
}Point;

typedef struct {
	Point coords;
}Macroblock;

typedef struct {
	Point coordsThis;
	Point coordsFrom;
}Matchedblock;

uint8_t* getYComponent_YUV422_YUYV(uint8_t* image, int frameSize, int width, int height);

uint8_t* getFrameFromVideo(char* fileForRead, int frameSize, int frameNumber);
Point getBlock(uint8_t* image, int width, int height, int blockSize, Point blockMiddle, uint8_t* blockBuffer);
float calculateMAD(uint8_t* AblockBuffer, uint8_t* BblockBuffer, int blockSize);
Point getBestMatch(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize, int step, int WIDTH, int HEIGHT);
Point* blockMatchingTSS(Point* matches,uint8_t* currentFrame, uint8_t* prevFrame, int blockSize, int step, int WIDTH, int HEIGHT);

#endif // !_UTILS_H_
