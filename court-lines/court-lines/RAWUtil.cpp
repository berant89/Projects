#include "RAWUtil.h"

#include <iostream>

/**
 * @brief Loads a 8-bit RAW image from a given filename.
 * @param input The name of the file to be read into memory.
 * @param size The size of the file to be read.
 * @return A 2D vector of the image.
 */
Matb load_raw(const char* input, int imageX, int imageY)
{
	int size = imageX * imageY;
	//Load the image
	BYTE* buffer = new BYTE[size];
	FILE* fp = fopen(input, "rb");
	if(fp)
	{
		fread(buffer, size, 1, fp);
		fclose(fp);
	}
	else
	{
		delete[] buffer; //Delete the buffer and it's content.
		std::cerr<<"Failed to open file: "<<input<<std::endl;
		exit(1);
	}

	//Create a 2D vector for easy manipulation of the image.
	Matb image(imageX, Vecb(imageY-6));

	//Copy the content from the buffer to the vector using row stride.
	for(int i = 0; i < imageX; ++i)
		for(int j = 0; j < imageY-6; ++j)
			image[i][j] = buffer[i*imageY+(j+4)];
	
	delete[] buffer;

	return image;
}

/**
 * @brief Creates and writes an 8-bit RAW image with a given filename.
 * @param filename The name of the RAW output.
 * @param image 2D vector of the image.
 */
void write_raw(const char* filename, Matb image)
{
	int rows = image.size(), cols = image[0].size(), size = rows*cols;
	BYTE* buffer = new BYTE[size]; //Create a new buffer from the size of the image.

	//Copy the data from the vector and store it into the buffer using row stride.
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			buffer[i*cols+j] = image[i][j];

	//Create the file with the given filename.
	FILE *fp = fopen(filename, "wb");

	//Check if it opened.
	if(fp)
	{
		fwrite(buffer, size, 1, fp);
		fclose(fp);
		delete[] buffer;
	}
	else
	{
		std::cerr<<filename<<" failed to open to be written to.\n";
		delete[] buffer;
		exit(1);
	}
}

/**
 * @brief Creates and writes to a text file the coordinates of the given image.
 * @param filename The name of the text file.
 * @param image 2D vector of the image.
 */
void write_coord(const char* filename, Matb image)
{
	int rows = image.size(), cols = image[0].size();
	//Create the file with the given filename.
	FILE* fp = fopen(filename, "w");

	//Check if it opened.
	if(fp)
	{
		//Go through vector.
		for(int i= 0; i < rows; ++i)
		{
			for(int j = 0; j < cols; ++j)
			{
				//IF we see a white pixel then write to the file its location.
				if(image[i][j] == 255)
					fprintf(fp, "%d %d\n", i, j);
			}
		}
		fclose(fp);
	}
	else
	{
		std::cerr<<filename<<" failed to open to be written to.\n";
		exit(1);
	}
}