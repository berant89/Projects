#include "alphanum2.h"

#include <locale>

using namespace std;


int compareRight(string a, string b)
{
	int bias = 0;
	int ia = 0;
	int ib = 0;

	// The longest run of digits wins. That aside, the greatest
	// value wins, but we can't know that it will until we've scanned
	// both numbers to know that they have the same magnitude, so we
	// remember it in BIAS.
	for (;; ia++, ib++)
	{
		char ca = charAt(a, ia);
		char cb = charAt(b, ib);

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
		else if (ca < cb)
		{
			if (bias == 0)
			{
				bias = -1;
			}
		}
		else if (ca > cb)
		{
			if (bias == 0)
				bias = +1;
		}
		else if (ca == 0 && cb == 0)
		{
			return bias;
		}
	}
}

int compare(string a, string b)
{
	int ia = 0, ib = 0;
	int nza = 0, nzb = 0;
	char ca, cb;
	int result;

	while (true)
	{
		// only count the number of zeroes leading the last number compared
		nza = nzb = 0;

		ca = charAt(a, ia);
		cb = charAt(b, ib);

		// skip over leading spaces or zeros
		while (isspace(ca) || ca == '0')
		{
			if (ca == '0')
			{
				nza++;
			}
			else
			{
				// only count consecutive zeroes
				nza = 0;
			}

			ca = charAt(a, ++ia);
		}

		while (isspace(cb) || cb == '0')
		{
			if (cb == '0')
			{
				nzb++;
			}
			else
			{
				// only count consecutive zeroes
				nzb = 0;
			}

			cb = charAt(b, ++ib);
		}

		// process run of digits
		if (isdigit(ca) && isdigit(cb))
		{
			if ((result = compareRight(a.substr(ia), b.substr(ib))) != 0)
			{
				return result;
			}
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

static char charAt(string s, int i)
{
	if (i >= s.length())
	{
		return 0;
	}
	else
	{
		return s.at(i);
	}
}