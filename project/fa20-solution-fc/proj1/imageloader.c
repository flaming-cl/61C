/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				flaming-cl
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

#define FILE_LOCATION "imageloader"
int PIXEL_IN_TOTAL_IMAGELOAD;


//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *imageDoc = fopen(filename, "r");
	Image *img = (Image*) malloc(sizeof(Image));
	if (fileParamsInvalid(imageDoc, img)) {
		freeMemoryAndExit(imageDoc, img);
	}

	constructsImageObj(imageDoc, img);
	fclose(imageDoc);

	return img;
}

// readData - varifies the .ppm P3 image file
int fileParamsInvalid(FILE *imageDoc, Image *img) {
	int fileInvalid = 1;
	if (!imageDoc) {
		showError(ERROR_NO_IMAGE_FILE, FILE_LOCATION);
		return fileInvalid;
	}

	if (!img) {
		showError(ERROR_WHEN_MALLOC, FILE_LOCATION);
		return fileInvalid;
	}
	
	char imageFormat[FORMAT_DIGIT];
	int maxVal;

	fscanf(imageDoc, "%s %u %u %u", imageFormat, &img->cols, &img->rows, &maxVal);
	// strcmp - two strings not equal - result 1
	if (maxVal != MAX_COLOR_VAL || !img->cols || !img->rows || strcmp(imageFormat, FILE_FORMAT)) {
		showError(ERROR_INVALID_FILE_PARAM, FILE_LOCATION);
		return fileInvalid;
	}

	PIXEL_IN_TOTAL_IMAGELOAD = img->rows * img->cols;
	img->image = (Color**) malloc(sizeof(Color*) * PIXEL_IN_TOTAL_IMAGELOAD);
	if (!img->image) {
		showError(ERROR_WHEN_MALLOC, FILE_LOCATION);
		return fileInvalid;
	}
	
	fileInvalid = 0;
	return fileInvalid;
}

// constructs an Image object
void constructsImageObj(FILE *imageDoc, Image *img) {
	int i;
	for (i = 0; i < PIXEL_IN_TOTAL_IMAGELOAD; i++) {
		// get a question here: if I malloc A and make B (a pointer) = A, I only need to free A or B in the end, right?
		img->image[i] = (Color*) malloc(sizeof(Color));
		if (!img->image[i]) freeMemoryAndExit(imageDoc, img);
		Color* p = img->image[i];
		fscanf(imageDoc, "%hhu %hhu %hhu", &p->R, &p->G, &p->B); // %hhu for unsigned int
	}
}

// show error message
void showError(char* generalMsg, char* locationMsg) {
	printf("%s %s", generalMsg, locationMsg);
}

// if any error occurs, free the image, close the image file and exit with error code - 1
void freeMemoryAndExit(FILE *imageDoc, Image *img) {
	if (img) freeImage(img);
	if (imageDoc) fclose(imageDoc);
	exit(ERROR_CODE);
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{	
	if (!image) {
		showError(ERROR_INVALID_FILE_PARAM, FILE_LOCATION);
		exit(ERROR_CODE);
	}

	printf("%s\n%d %d\n%d\n", FILE_FORMAT, image->cols, image->rows, MAX_COLOR_VAL);
	int i;
	for (i = 0; i < PIXEL_IN_TOTAL_IMAGELOAD; i++) {
		Color* p = image->image[i];
		if ((i + 1) % image->cols == 0) {
			printf("%3hhu %3hhu %3hhu\n", p->R, p->G, p->B);
		} else {
			printf("%3hhu %3hhu %3hhu   ", p->R, p->G, p->B);
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->cols * image->rows; i++) {
		Color* p = image->image[i];
		if (!p) break;
		free(p);
	}
	free(image->image);
	free(image);		
}