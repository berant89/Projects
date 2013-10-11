#include "alphanum.h"

#include <locale> //std::isdigit, std::isspace

using namespace std;

/**
 * @brief Returns the character from a string.
 * @param s The string.
 * @param i The position.
 * @return The character from the string.
 */
static char charAt(string s, int i)
{
	//Check that the position is not out of bounds.
	if (i >= s.length())
	{
		return 0;
	}
	else
	{
		return s.at(i);
	}
}

/**
 * @brief Compares the string the first string to the second string.
 * @param a The first string.
 * @param b The second string.
 * @return The boolean result of the comparison.
 */
int compareRight(string a, string b)
{
	int bias = 0; //To indicate which string is next on the sorted list.
	int ia = 0; //Counter for string a.
	int ib = 0; //Counter for string b.

	/*
	The longest run of digits wins. That aside, the greatest
	value wins, but we can't know that it will until we've scanned
	both numbers to know that they have the same magnitude, so we
	remember it in BIAS.
	*/
	for(;; ia++, ib++)
	{
		char ca = charAt(a, ia); //Character of sting a at ia.
		char cb = charAt(b, ib); //Character of sting b at ib.

		//Check if the current characters are digits
		if (!isdigit(ca) && !isdigit(cb))
		{
			return bias;
		}
		else if (!isdigit(ca))
		{
			return -1;
		}
		else if (!isdigit(cb))
		{
			return +1;
		}
		//The characters are digits.
		else if (ca < cb)
		{
			if (bias == 0)
				bias = -1;
		}
		else if (ca > cb)
		{
			if (bias == 0)
				bias = +1;
		}
		//No change in bias.
		else if (ca == 0 && cb == 0)
		{
			return bias;
		}
	}
}

int compare(string a, string b)
{
	int ia = 0, ib = 0; //Counters for the strings.
	int nza = 0, nzb = 0; //Number of zeros.
	char ca, cb; //Character for the strings.
	int result; //Result of the comparison.

	while(true)
	{
		//Only count the number of zeroes leading the last number compared.
		nza = nzb = 0;

		//Get the character at ia and ib.
		ca = charAt(a, ia);
		cb = charAt(b, ib);

		//Skip over leading spaces or zeros
		while(isspace(ca) || ca == '0')
		{
			if (ca == '0')
				nza++;
			else
				nza = 0; //Only count consecutive zeroes.

			ca = charAt(a, ++ia);
		}

		//Skip over leading spaces or zeros
		while (isspace(cb) || cb == '0')
		{
			if (cb == '0')
				nzb++;
			else
				nzb = 0; //Only count consecutive zeroes

			cb = charAt(b, ++ib);
		}

		//Process run of digits.
		if (isdigit(ca) && isdigit(cb))
		{
			if ((result = compareRight(a.substr(ia), b.substr(ib))) != 0)
				return result;
		}

		if (ca == 0 && cb == 0)
		{
			// The strings compare the same. Perhaps the caller
			// will want to call strcmp to break the tie.
			return nza - nzb;
		}

		if (ca < cb)
		{
			return -1;
		}
		else if (ca > cb)
		{
			return +1;
		}

		++ia;
		++ib;
	}
}