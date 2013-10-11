#include "LineUtil.h"

#include <iostream>

/**
 * @brief Loads a 8-bit RAW image from a given filename.
 * @param input The name of the file to be read into memory.
 * @param size The size of the file to be read.
 */
std::vector< std::vector<BYTE> > load_raw(char* input, int imageX, int imageY)
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
	std::vector< std::vector<BYTE> > image(imageX, std::vector<BYTE>(imageY));

	for(int i = 0; i < imageX; ++i)
		for(int j = 0; j < imageY; ++j)
			image[i][j] = buffer[i*imageY+j];
	
	delete[] buffer;

	return image;
}

/**
 * @brief Creates and writes an 8-bit RAW image with a given filename.
 * @param filename The name of the RAW output.
 * @param size The size of the input buffer.
 * @param buffer A pointer to the image data.
 */
void write_raw(char* filename, BYTE* buffer, int size)
{
	/*int size = image.size()*image[0].size();
	BYTE* buffer = new BYTE[size];*/
	//Load image data.
	FILE *fp = fopen(filename, "wb");

	//Check if it opened.
	if(fp)
	{
		fwrite(buffer, size, 1, fp);
		fclose(fp);
		//delete[] buffer;
	}
	else
	{
		std::cerr<<filename<<" failed to open to be written to.\n";
		//delete[] buffer;
		exit(1);
	}
}