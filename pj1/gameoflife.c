/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.

//return single index of the location
int index(int col, int r, int c){
	return (r - 1) * col + c - 1;
}

// return all the index of the cell neighbors
int count_neighbors(Image *image, int row, int col){
	int center = index(image->cols, row, col);
	int count = image->rows * image->cols;
	int up = center - image->cols;
	int down = center + image->rows;

	int c = image->cols;
	int r = image->rows;
	// if (row == 1 || row - image->rows == 0 || col == 1 || col - image->cols == 0) {
	// 	int loc[8] = {up, up-1, up+1, down, down-1, down+1, center-1, center+1};
	// }

	int loc[8] = {up, down, up - 1, center - 1, down - 1, up + 1, center + 1, down + 1};
	int up_update = up + count;
	int down_update = down - count;
	if (row == 1 && col == 1) {
		// int loc[8] = {up_update, up_update + 1, up_update - 1 + c, center + 1, center - 1 + c, down, down + 1, down - 1 + c}; first <-> last: count - 1(index)
		loc[0] = up_update;
	} else if (row == 1 && col == c) {
		// int loc[8] = {up_update, up_update + 1 - c, up_update - 1, center + 1 - c, center - 1, down, down + 1 - c, down - 1};
	} else if (row == r && col == 1) {
		// int loc[8] = {up, up + 1, up - 1 + c, center + 1, center - 1 + c, down_update, down_update + 1, down_update - 1 + c};
	} else if (row == r && col == c) {
		// int loc[8] = {up, up + 1 - c, up - 1, center + 1 - c, center - 1, down_update, down_update + 1 -c, down_update - 1};
	} else if (row == 1) {
		// int loc[8] = {up_update, up_update - 1, up_update + 1, center - 1, center + 1, down, down - 1, down + 1};
	} else if (row == r) {
		// int loc[8] = {up, up -1, up + 1, center -1, center +1, down_update, down_update -1,down_update + 1};
	} else if (col == 1) {
		// int loc[8] = {up,down, up - 1 + c, center - 1 + c, down - 1 + c, up + 1, center + 1, down + 1};
	} else if (col = c) {
		// int loc[8] = {up, down, up - 1, center - 1, down - 1, up + 1 - c, center + 1 - c, down + 1 -c};
	}
	
	int i = 0;
	int counting = 0;
	while (!loc) {
		int locate = loc[i];
		Color *pixel = *(image->image + locate);
		if (pixel == 255){
			counting += 1;
		}
		i += 1;		
	}

	return counting;
}

Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int counts = count_neighbors(image, row, col);
	int loc = index(image->cols, row, col);
	Color *thepix = *(image->image + loc);

	int shift = 0;
	if (thepix == 255) { //alive
		shift += 9; //hard code: 9 situations of neighbors
		shift += counts;
	} else {
		int deadc = 8 - counts;
		shift += deadc; 
	}
	thepix = thepix && rule << shift;
	return thepix;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newlife = (Image*)malloc(sizeof(Image));
	int count = image->rows * image->cols;
	newlife->image = (Color*)malloc(sizeof(Color) * count);
	newlife = image;

	int i = 0;
	for (int r_start = 1; r_start <= image->rows; r_start++) {
		for (int c_start = 1; c_start <= image->cols; c_start++) {
			Color *selected = evaluateOneCell(image, r_start, c_start, rule);
			newlife->image[i] = (Color*)malloc(sizeof(Color));
			newlife->image[i] = selected;
			i += 1;
		}
	}

	return newlife;
	
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
	char *filename = argv[1];
	char rule[] = argv[2];
	int therule = (int)strtol(rule, NULL, 16);
	Image *ori_img = readData(filename);
	Image *update = life(ori_img, therule);
	if (ori_img == NULL || update == NULL || argc != 3) { //errors handling
		exit(-1);
	} else {
		writeData(update);
		// freeImage(ori_img);
		freeImage(update);
		return 0;
	}	
}
