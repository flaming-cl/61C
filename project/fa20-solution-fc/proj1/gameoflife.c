/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				flaming-cl
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

#define FILE_LOCATION "gameoflife"
#define GAME_BASE 16
#define RULE_BITS 18
#define HALF_RULE_BITS 9
#define NEIGHBOR_LIMIT 8
#define STATUS_TYPE 2
#define CHECK_BIT(var,pos) ((var) & (1 << (pos)))

int NEIGHBOR_POS[NEIGHBOR_LIMIT][STATUS_TYPE] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
int PIXEL_IN_TOTAL_LIFE;
int DECEASED_CELL_NEXT_STATES[HALF_RULE_BITS];
int ALIVE_CELL_NEXT_STATES[HALF_RULE_BITS];

void hexToBitsArray(uint32_t rule) {
	int i;
	for (i = 0; i < RULE_BITS; i++) {
		if (i >= (HALF_RULE_BITS)) {
			ALIVE_CELL_NEXT_STATES[i - HALF_RULE_BITS] = (rule >> i) & 1;
		} else {
			DECEASED_CELL_NEXT_STATES[i] = (rule >> i) & 1;
		}
    }
}

void mallocFailed(Image *imageCurrentGen, Image *imageNextGen, int shouldFreeNextImage) {
	showError(ERROR_WHEN_MALLOC, FILE_LOCATION);
	free(imageCurrentGen);
	if (shouldFreeNextImage) freeImage(imageNextGen);
	exit(ERROR_CODE);
}

int getPosInEachDirection(int currentPos, int index, int base, int isCol) {
	int newRowPos = currentPos + NEIGHBOR_POS[index][isCol];
	if ((newRowPos > -1) && (newRowPos <= base)) {
		// check if the pos is out of the image boundry
		return (newRowPos + base) % base;
	}
	return -1;
}

void countNeighbors(Image *image, int row, int col, int liveNeighbors[]) {
    int rows = image->rows;
    int cols = image->cols;

    for (int i = 0; i < NEIGHBOR_LIMIT; i++) {
        int nRow = getPosInEachDirection(row, i, rows, 0);
        int nCol = getPosInEachDirection(col, i, cols, 1);
		if (nRow < 0 || nCol < 0) continue;
		liveNeighbors[0] += (image->image[nRow * cols + nCol])->R == 255;
		liveNeighbors[1] += (image->image[nRow * cols + nCol])->G == 255;
		liveNeighbors[2] += (image->image[nRow * cols + nCol])->B == 255;
    }
}

void findAliveRGB(Image *image, int row, int col, int aliveRGBCounts[]) {
	aliveRGBCounts[0] = (image->image[row * image->cols + col])->R == 255;
	aliveRGBCounts[1] = (image->image[row * image->cols + col])->G == 255;
	aliveRGBCounts[2] = (image->image[row * image->cols + col])->B == 255;
}

void getRGBAddress(Color *nextGen, uint8_t **p) {
	p[0] = &nextGen->R;
	p[1] = &nextGen->G;
	p[2] = &nextGen->B;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	hexToBitsArray(rule);

	Color *nextGen = (Color*) malloc(sizeof(Color));
	if (!nextGen) return NULL;

	uint8_t **p = (uint8_t **) malloc(sizeof(uint8_t *) * 3);
	if (!p) return NULL;
	getRGBAddress(nextGen, p);

	int neighborCounts[3] = {0, 0, 0};
	countNeighbors(image, row, col, neighborCounts);
	
	int aliveRGBCounts[3] = {0, 0, 0};
	findAliveRGB(image, row, col, aliveRGBCounts);
	
	int i;
	for (i = 0; i < 3; i++) {
		if (aliveRGBCounts[i] == 0) {
			int nextGenAlive = DECEASED_CELL_NEXT_STATES[neighborCounts[i]];
			*(p[i]) = nextGenAlive ? 255 : 0;
		}
		if (aliveRGBCounts[i] == 1) {
			int nextGenAlive = ALIVE_CELL_NEXT_STATES[neighborCounts[i]];
			*(p[i]) = nextGenAlive ? 255 : 0;
		}
	}

	free(p);
	return nextGen;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newGenerationImg = (Image*) malloc(sizeof(Image));
	if (!newGenerationImg) mallocFailed(image, newGenerationImg, NOT_TO_FREE_IMAGE);

	newGenerationImg->rows = image->rows;
	newGenerationImg->cols = image->cols;
	PIXEL_IN_TOTAL_LIFE = newGenerationImg->rows * newGenerationImg-> cols;
	
	newGenerationImg->image = (Color**) malloc(sizeof(Color*) * PIXEL_IN_TOTAL_LIFE);
	if (!newGenerationImg->image) mallocFailed(image, newGenerationImg, TO_FREE_IMAGE);

	int i;
	for (i = 0; i < PIXEL_IN_TOTAL_LIFE; i++) {
		newGenerationImg->image[i] = evaluateOneCell(
			image,
			i / image->cols,
			i % image->cols,
			rule
		);
		if (!newGenerationImg->image[i]) {
			mallocFailed(image, newGenerationImg, TO_FREE_IMAGE);
		}
	}
	return newGenerationImg;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc < 3) {
		showError(ERROR_INPUT_ARGC, FILE_LOCATION);
		exit(ERROR_CODE);
	}
	
	Image *image = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, GAME_BASE);

	Image *newGenerationImg = life(image, rule);
	writeData(newGenerationImg);

	freeImage(image);
	freeImage(newGenerationImg);	
	
	return 0;
}

