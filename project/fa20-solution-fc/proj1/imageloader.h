#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define FORMAT_DIGIT 3
// FORMAT_DIGIT - P, 3, \0
#define MAX_COLOR_VAL 255
#define FILE_FORMAT "P3"
#define ERROR_INPUT_ARGC "Error: invalid argc."
#define ERROR_NO_IMAGE_FILE "Error: can not open image file."
#define ERROR_INVALID_FILE_PARAM "Error: invalid file param."
#define ERROR_WHEN_MALLOC "Error: no enough space to malloc."
#define ERROR_CODE -1
#define TO_FREE_IMAGE 1
#define NOT_TO_FREE_IMAGE 0

typedef struct Color 
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} Color;
typedef struct Image
{
	Color **image;
	uint32_t rows;
	uint32_t cols;
} Image;

int fileParamsInvalid(FILE *imageDoc, Image *image);

void constructsImageObj(FILE *imageDoc, Image *image);

void freeMemoryAndExit(FILE *imageDoc, Image *img);

extern void showError(char* generalMsg, char* locationMsg);

//Loads an image from a ppm file, and converts to an Image object
extern Image *readData(char *filename);

//Writes an image to stdout
extern void writeData(Image *image);

//Frees an image and all its associated data
extern void freeImage(Image *image);
