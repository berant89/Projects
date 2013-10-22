#pragma once
#include <vector>

typedef unsigned char BYTE;

typedef std::vector<BYTE> Vecb;
typedef std::vector< Vecb > Matb;
typedef std::vector<int> Veci;
typedef std::vector< Veci > Mati;
typedef std::vector<double> Vecd;
typedef std::vector< Vecd > Matd;

typedef std::pair<int, int> Coord;
typedef std::pair< Coord, Coord > Coords;
typedef std::vector< Coords > Points;