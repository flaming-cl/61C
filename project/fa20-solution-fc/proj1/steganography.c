/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				flaming-cl
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

#define FILE_LOCATION "steganorgraphy"

int PIXEL_IN_TOTAL_STEGANO;

// when malloc fails => freeImage/exit
void mallocFailed(Image *image, int shouldFreeImage) {
	showError(ERROR_WHEN_MALLOC, FILE_LOCATION);
	if (shouldFreeImage) freeImage(image);
	exit(ERROR_CODE);
}

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
//Notice: I changed the function a little bit. I don't think I must past rows and cols here.
Color *evaluateOnePixel(Image *image, Color *originalColor)
{
	Color* revealPixel = (Color*) malloc(sizeof(Color));
	if (!revealPixel) mallocFailed(image, TO_FREE_IMAGE);
	int msgColor = (originalColor->B % 2) * 255;
	revealPixel->R = revealPixel->G = revealPixel->B = msgColor;
	return revealPixel;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *imageBChannel = (Image*) malloc(sizeof(Image));
	if (!imageBChannel) mallocFailed(image, NOT_TO_FREE_IMAGE);

	imageBChannel->rows = image->rows;
	imageBChannel->cols = image->cols;
	PIXEL_IN_TOTAL_STEGANO = imageBChannel->rows * imageBChannel-> cols;
	
	imageBChannel->image = (Color**) malloc(sizeof(Color*) * PIXEL_IN_TOTAL_STEGANO);
	if (!imageBChannel->image) mallocFailed(image, TO_FREE_IMAGE);

	int i;
	for (i = 0; i < PIXEL_IN_TOTAL_STEGANO; i++) {
		imageBChannel->image[i] = evaluateOnePixel(image, image->image[i]);
	}
	return imageBChannel;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc < 2) {
		showError(ERROR_INPUT_ARGC, FILE_LOCATION);
		exit(ERROR_CODE);
	}
	Image *image = readData(argv[1]);
	Image *imageBChannel = steganography(image);
	writeData(imageBChannel);

	freeImage(imageBChannel);
	freeImage(image);
	return 0;
}
