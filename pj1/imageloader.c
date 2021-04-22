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
	FILE *fp = fopen(filename, "r");
	Image *img = (Image*)malloc(sizeof(Image));	
	char format[2];
	int val;
	fscanf(fp, "%s %u %u %u",format, &(img->cols), &(img->rows), &val);

	uint32_t count = img->rows * img->cols;
	img->image = (Color**)malloc(sizeof(Color*) * count);
	int i = 0;
	while (i < count) {
		img->image[i] = (Color*)malloc(sizeof(Color));
		Color *pix = *(img->image + i);
		fscanf(fp, "%hhu %hhu %hhu", &(pix->R), &(pix->G), &(pix->B));
		i += 1;
	}
	fclose(fp);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("%u\n", 255);

	uint32_t count = image->rows * image->cols;
	int i = 0;
	while (i < count) {
		Color *pix = *(image->image + i);
		printf("%3hhu %3hhu %3hhu", pix->R, pix->G, pix->B);
		if ((i + 1) % image->cols == 0){
			printf("\n");
		} else {
			printf("   ");
		}
		i += 1;
	}
}

//Frees an image
void freeImage(Image *image)
{
	int i = 0;
	while (i < image->rows * image->cols){
		Color *pix = *(image->image + i);
		if (!pix){
			break;
		} else {
			free(pix);
		}
		i += 1;
	}
	free(image->image);
	free(image);
}
