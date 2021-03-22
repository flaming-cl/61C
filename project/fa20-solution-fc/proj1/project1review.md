### Summary for Project1:
I feel this assignment is mainly about C memory management and pointers. So knowing what they've taught about C through the course is enough.
### A1: load image
#### 1. read an image file by its filename
#### 2. create an Image struct and fill your Image struct with data from the input image file.
```c
typedef struct Image
{
    Color **image;
    uint32_t rows;
    uint32_t cols;
} Image;

Image *img // Image *img: your image struct
img->image  // Color **image: the malloc pointer to begin storing your Color pixels 
*(img->image+i) // Color *pixel: aka every Color struct which contains your RGB channels
```
#### 3. wirteData
I got confused by the function name at the every beginning, but I realized this is just for printing the Color pixels. Aka the value of your RGB channels

#### 4. freeImage
Remember that you’ve allocated some memory via malloc. Ain't no free lunch, this is for freeing the memory you’ve used.  

#### Tips:
When printing your data, be careful about those tricky space/newline requirements. 
> **Note that some of the examples in testInputs and most converters will have different amounts of whitespace between pixels and the end of lines**. If you use fscanf, though, your code should be able to handle different amounts of whitespace between pixels.


### A2: Steganography
I am so delighted that the course offers a glimpse of a few inspiring topics like Steganography. It just reminded me of some news about hackers hiding malware inside pictures. If you have further interest in this topic, you can take a look at the following articles:
- [Security Researcher Hides ZIP, MP3 Files Inside PNG Files on Twitter](https://threatpost.com/researcher-hides-files-in-png-twitter/164881/)
- [How to Hide Secret Data Inside an Image or Audio File in Seconds](https://null-byte.wonderhowto.com/how-to/steganography-hide-secret-data-inside-image-audio-file-seconds-0180936/)

#### background information
1B is not a challenging task, you can quickly implement it once you understand how to extract the LSB data.
![steganography](https://user-images.githubusercontent.com/51183663/111978564-ffc0f100-8b3e-11eb-9277-b5a2df2c226c.jpg)
#### STEP 1: get the least significant bit of each pixel's blue channel
> 1 1 1 0
#### STEP 2: tranform the least significant bit into black/white
> 255 255 255 0   
  
NOTE: here we are replacing the red/green channel and the other 7 bits of the blue channel with **the least bit of the blue channel**.


### B: The Game of Life
Tip: we only need to spend time reading the state table, not the Hex rule table. It really doesn’t matter which Hex rule you are applying—the core idea behind the rules is the same.
![state table](https://user-images.githubusercontent.com/51183663/111978668-23843700-8b3f-11eb-9782-3fce0ebc39f8.png)
#### State table
1. current state
The state of each cell is either alive or dead.

2. next state
If an alive cell has 2/3 alive neighbors, it will stay alive next. Otherwise, it will die.
If a dead cell has 3 alive neighbors, it will become alive next. Otherwise, it will remain dead.

#### Hex rule table
An example of Replicator 0x154AA:     
``` c
8 7 6 5 4 3 2 1 0 || 8 7 6 5 4 3 2 1 0         
0 1 0 1 0 1 0 1 0 || 0 1 0 1 0 1 0 1 0        
```
If an alive cell has 7/5/3/1 alive neighbors, it will stay alive next. Otherwise, it will die.
If a dead cell has 7/5/3/1 alive neighbors, it will become alive next. Otherwise, it will remain dead.

#### As you can tell, to get the next state of a cell, we need to know two things:
1) whether the cell is alive or not
This is quite simple: if a channel of a pixel is alive, its value will be 255. (Right, each channel is a cell here). To get the value, just read from the input ppm file.

2) how many alive neighbors does it have
Although we are using a linear data structure to store our cell data, we have to treat it as a matrix. So, how can we count the alive neighbors of a certain cell?
3) access its 8 surrounding cells (ideally) 
![pos-matrix](https://user-images.githubusercontent.com/51183663/111976590-c25b6400-8b3c-11eb-9696-0b5d4ec3b0fe.jpg)
4) if a surrounding cell is a alive, aliveNeighbor++

Tip: DON’T CHANGE the state of each cell in the original image.

## C programming notes
1. check if a specific bit of a hex number is 0
```c
if (rule & (1 << bit) == 0)
```
2. allocate an array of pointers
```c
int len = 3;
uint8_t **p = (uint8_t **) malloc(sizeof(uint8_t *) * len);
```
3. format specifiers
https://www.geeksforgeeks.org/format-specifiers-in-c/