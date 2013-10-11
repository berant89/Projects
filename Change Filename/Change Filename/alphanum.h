#pragma once
/**
 * This implements a comparator for human/natural sorting. Since most programs
 * use ASCII sorting this can ruin the results for lets say renaming files.
 */

#include <string>

int compare(std::string a, std::string b);