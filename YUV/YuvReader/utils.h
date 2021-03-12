#ifndef _UTILS_H_
#define _UTILS_H_

#define PI 3.14159265

#define WIDTH 1280
#define HEIGHT 720
#define BLOCK_SIZE 32
#define BLOCK_ELEMENTS (BLOCK_SIZE*BLOCK_SIZE)
#define RANDOM(upper,lower) ((rand() % (upper - lower + 1)) + lower)
typedef struct {
	int x;
	int y;
}Point;

typedef struct {
	Point coords;
}Macroblock;

typedef struct {
	Point from;
	Point to;
	int length;
	int angle;
}Vector;

typedef struct {
	int xfrom;
	int yfrom;
	int xto;
	int yto;
	int length;
	int angle;
	VectorItem* next;
}VectorItem;
struct Item {
	int xfrom;
	int yfrom;
	int xto;
	int yto;
	int length;
	int angle;
	Item* next;
};
typedef struct konj k;
struct k {
	int xfrom;
	int yfrom;
	int xto;
	int yto;
	int length;
	int angle;
	k* next;
};

typedef struct {
	VectorItem* head;
	int itemsNum;
}VectorsList;
void init9PointsForLDSP(Point center, Point* points);
void init4PointsForSDSP(Point center, Point* points);
int calculateLength2Points(Point from, Point to);
int calculateAngle2Points(Point to, Point from);
void filterVectorsForDuplicationError(Vector* vectors, int width, int height, int blockSize);
void filterVectorsByOrientation(Vector* vectors, int blockSize);
uint8_t* getYComponent_YUV422_YUYV(uint8_t* image, int frameSize, int width, int height);
uint8_t* getYComponent_YUV420P(char* filepath, int width, int height);
uint8_t* getFrameFrom422YUYVVideo(char* fileForRead, int width, int height, int frameNumber);
uint8_t* getYComponent_YUV444_YUV(char* filepath, int width, int height);
Point getCenter(Point upperLeft);
Point getUpperLeft(Point center);
Point getBlock(uint8_t* image, int width, int height, int blockSize, Point currentPoint, uint8_t* blockBuffer);
long float calculateMAD(uint8_t* AblockBuffer, uint8_t* BblockBuffer, int blockSize);
Point searcherSDSP(Point center, uint8_t* refMacroblock, uint8_t* frameCurrent, float minMAD);
Point searcherLDSP(Point center, uint8_t* refMacroblock, uint8_t* frameCurrent, float minMAD);
Point getBestMatchTSS(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize, int step);
Point* blockMatchingTSS(Vector* matches,uint8_t* currentFrame, uint8_t* prevFrame, int blockSize, int step);
Point* blockMatchingEBMA(Vector* matches, uint8_t* currentFrame, uint8_t* prevFrame, int blockSize,int searchArea);
Point* blockMatchingDSP(Vector* matches, uint8_t* currentFrame, uint8_t* prevFrame);
Point getBestMatchEBMA(Point macroblockCoords, uint8_t* refMacroblock, uint8_t* frameCurrent, int blockSize, int searchArea);
Point* getSearchArea(Point macroblockCoords, int searchArea, int width, int height, int blockSize);

#endif // !_UTILS_H_
