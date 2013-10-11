#pragma once

typedef unsigned char BYTE;
#include <vector>

std::vector< std::vector<BYTE> > load_raw(char* input, int imageX, int imageY);
void write_raw(char* filename, BYTE* buffer, int size);