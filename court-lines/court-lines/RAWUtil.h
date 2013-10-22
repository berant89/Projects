#pragma once

#include "VecMatTypes.h"

Matb load_raw(const char*, int, int);
void write_raw(const char* filename, Matb);
void write_coord(const char*, Matb);