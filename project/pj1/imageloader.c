/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	uint32_t* rows;
	uint32_t* cols;
	uint32_t* scale;
	Color **image;
	
	FILE *fp = fopen(filename, "r");
	fscanf(fp, "%u %u %u", rows, cols, scale);
	uint32_t count = *rows * *cols;
	Color colors[count];
	Color current;
	uint8_t *R, *G, *B;
	int i = 0;
	while (i < count){
		fscanf(fp, "%hhu %hhu %hhu", R, G, B);
		current = colors[i];
		current.R = *R;
		current.G = *G;
		current.B = *B;
		i += 1;
	}
	
	image = colors;
	fclose(fp);
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	uint32_t rows;
	uint32_t cols;
	uint32_t scale;
	Color *curs;
	rows = image->rows;
	cols = image->cols;
	curs = image->image;
	scale = 255;

	printf("P3\n");
	printf("%u %u\n", rows, cols);
	printf("%u\n", scale);

	int r = 0;
	int c = 0;
	int i = 0;
	while (c < cols){
		while (r < rows){
			Color curr = curs[i];
			printf("%u %u %u", curr.R, curr.G, curr.B);
			if (r == rows - 1){
				printf("\n");
			} else {
				printf("   ");
			}
			r += 1;
			i += 1;
		}
		c += 1;
	}
	printf("\n");

}

//Frees an image
void freeImage(Image *image)
{
	Image *countImage = (Image*) malloc(sizeof(Image));
	countImage->image = malloc(sizeof(uint8_t) * image->rows * image->cols);
	writeData(image);

	free(image->image);
	free(countImage);
}