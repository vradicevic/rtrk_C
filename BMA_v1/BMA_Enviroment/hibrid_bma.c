#include "hibrid_bma.h"




int blockMatchingHBMA(int16_t** vectors, uint8_t* currentFrame, uint8_t* prevFrame, int searchArea) {
	Point prevMacroblockCoo;
	int deviation, x, y;
	Point privFrom, privTo;
	int index = 0;
	Point predicted;



	for (y = 0; y < HEIGHT - (HEIGHT % BLOCK_SIZE); y += BLOCK_SIZE) {
		for (x = 0; x < WIDTH - ((WIDTH % BLOCK_SIZE)); x += BLOCK_SIZE) {
			prevMacroblockCoo.x = x;
			prevMacroblockCoo.y = y;
			prevMacroblockCoo = getCenter(prevMacroblockCoo);
			prevMacroblockCoo = getUpperLeft(prevMacroblockCoo);
			privFrom = prevMacroblockCoo;

			//int median = getMedianOfBlock(currentFrame, prevMacroblockCoo);
			deviation = blockValueDeviation(currentFrame, prevMacroblockCoo);

			if (((float)deviation) / (BLOCK_SIZE * BLOCK_SIZE) > 5.0) {
				//printf("Deviation: %d\n", deviation);
				//float varin = ((float)deviation)/(BLOCK_SIZE*BLOCK_SIZE);
				//Vps_printf("Variance deviation %f", varin);
				if (index != 0) {
					prevMacroblockCoo = getCenter(prevMacroblockCoo);

					predicted.x = prevMacroblockCoo.x + (vectors[2][index - 1] - vectors[0][index - 1]);
					predicted.y = prevMacroblockCoo.y + (vectors[3][index - 1] - vectors[1][index - 1]);
					predicted = getUpperLeft(predicted);
					prevMacroblockCoo = getUpperLeft(prevMacroblockCoo);
					//printf("predicted.x %d; predicted.y: %d, prev.x %d; prev.y %d\n", predicted.x, predicted.y, prevMacroblockCoo.x, prevMacroblockCoo.y);
				}
				else {
					predicted = prevMacroblockCoo;
				}

				privTo = getBestMatchHBMA(prevMacroblockCoo, currentFrame, prevFrame, searchArea, predicted);


			}
			else {
				privTo = prevMacroblockCoo;
			}


			int length = calculateLength2Points(privFrom, privTo);
			//if (length > 2) {
			vectors[0][index] = (int16_t)privFrom.x;
			vectors[1][index] = (int16_t)privFrom.y;
			vectors[2][index] = (int16_t)privTo.x;
			vectors[3][index] = (int16_t)privTo.y;
			vectors[4][index] = (int16_t)length;
			vectors[5][index] = CAST_ANGLE(calculateAngle2Points(privTo, privFrom));
			
			index++;
			//}
			




		}
	}

	return index;

}

Point getBestMatchHBMA(Point prevMacroblockCoo, uint8_t * currentFrame, uint8_t * prevFrame, int searchArea, Point predicted) {
	Point best;
	Point points[9];
	Point currentMacroblockCoo;
	int step = 3;

	/////blockSize*blockSize
	//////postavljanje na sredinu makrobloka
	
	int idBest = 0;

	float minMad = 99999.999;
	float MAD;
	//best = points[0];
	int i,x,y;

	int flag = 1;
	int skips[] = { 0,0,0,0,0,0,0,0,0 };
	Point pointsSA[2];
	getSearchArea(predicted, searchArea, pointsSA);

	for (y = pointsSA[0].y; y < pointsSA[1].y; y += 4) {
		for (x = pointsSA[0].x; x < pointsSA[1].x; x += 4) {
			currentMacroblockCoo.x = x;
			currentMacroblockCoo.y = y;

			MAD = calculateMAD(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);

			if (MAD < minMad) {
				minMad = MAD;
				best = currentMacroblockCoo;
			}
		}

	}
	best = getCenter(best);
	points[0] = best;
	//printf("Here i am");
	while (step >= 1) {
		points[1].x = points[0].x;              //up
		points[1].y = points[0].y - step;
		points[2].x = points[0].x;              //down
		points[2].y = points[0].y + step;
		points[3].x = points[0].x - step;           //left
		points[3].y = points[0].y;
		points[4].x = points[0].x + step;           //right
		points[4].y = points[0].y;
		points[5].x = points[0].x - step;       //upperLeft
		points[5].y = points[0].y - step;
		points[6].x = points[0].x + step;       //upperRight
		points[6].y = points[0].y - step;
		points[7].x = points[0].x - step;       //lowerLeft
		points[7].y = points[0].y + step;
		points[8].x = points[0].x + step;       //lowerRight
		points[8].y = points[0].y + step;

		idBest = 0;
		for (i = 0; i < 9; i++) {
			if (!skips[i]) {
				currentMacroblockCoo = getUpperLeft(points[i]);

				MAD = calculateMAD(currentFrame, prevFrame, currentMacroblockCoo, prevMacroblockCoo);

				if (MAD < minMad) {
					//printf("Iteration:%d \tMAD: %f\n", iteration, MAD);
					minMad = MAD;
					best = points[i];
					idBest = i;

				}
			}


		}
		//printf("Iteration:%d \tMAD: %f\n", iteration, minMad);

		points[0] = best;
		skips[0] = 1;
		if (idBest == 0) {
			//skips[0] = 0;
			skips[1] = 0;
			skips[2] = 0;
			skips[3] = 0;
			skips[4] = 0;
			skips[5] = 0;
			skips[6] = 0;
			skips[7] = 0;
			skips[8] = 0;
			step = step / 2;

		}
		else if (idBest == 1) {
			skips[1] = 0;
			skips[2] = 1;
			skips[3] = 1;
			skips[4] = 1;
			skips[5] = 0;
			skips[6] = 0;
			skips[7] = 1;
			skips[8] = 1;
		}
		else if (idBest == 2) {
			skips[1] = 1;
			skips[2] = 0;
			skips[3] = 1;
			skips[4] = 1;
			skips[5] = 1;
			skips[6] = 1;
			skips[7] = 0;
			skips[8] = 0;
		}
		else if (idBest == 3) {
			skips[1] = 1;
			skips[2] = 1;
			skips[3] = 0;
			skips[4] = 1;
			skips[5] = 0;
			skips[6] = 1;
			skips[7] = 0;
			skips[8] = 1;


		}
		else if (idBest == 4) {
			skips[1] = 1;
			skips[2] = 1;
			skips[3] = 1;
			skips[4] = 0;
			skips[5] = 1;
			skips[6] = 0;
			skips[7] = 1;
			skips[8] = 0;

		}
		else if (idBest == 5) {
			skips[1] = 0;
			skips[2] = 1;
			skips[3] = 0;
			skips[4] = 1;
			skips[5] = 0;
			skips[6] = 0;
			skips[7] = 0;
			skips[8] = 1;

		}
		else if (idBest == 6) {
			skips[1] = 0;
			skips[2] = 1;
			skips[3] = 1;
			skips[4] = 0;
			skips[5] = 0;
			skips[6] = 0;
			skips[7] = 1;
			skips[8] = 0;

		}
		else if (idBest == 7) {
			skips[1] = 1;
			skips[2] = 0;
			skips[3] = 0;
			skips[4] = 1;
			skips[5] = 0;
			skips[6] = 1;
			skips[7] = 0;
			skips[8] = 0;

		}
		else if (idBest == 8) {
			skips[1] = 1;
			skips[2] = 0;
			skips[3] = 1;
			skips[4] = 0;
			skips[5] = 1;
			skips[6] = 0;
			skips[7] = 0;
			skips[8] = 0;

		}

		
	}

	//printf("\nWinners %f ", minMad);
	best = getUpperLeft(best);
	if (minMad > 9) {
		best = prevMacroblockCoo;
	}
	return best;
}