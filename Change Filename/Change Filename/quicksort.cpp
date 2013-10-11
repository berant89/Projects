#include "quicksort.h"
#include "alphanum.h"

using namespace std;

/**
* @brief Swap the parameters.
* @param a The first parameter.
* @param a The second parameter.
*/
void swap(string& a, string& b)
{
    string temp = a;
    a = b;
    b = temp;
}

/**
* @brief Find and return the index of pivot element.
* @param a The array.
* @param first The start of the sequence.
* @param last The end of the sequence.
*/
int pivot(vector<string>& a, int first, int last) 
{
    int  p = first;
    string pivotElement = a[first];
 
    for(int i = first+1 ; i <= last ; i++)
    {
		//Use compare from alphanum.h
        if(compare(a[i], pivotElement) < 0)
        {
            p++;
            swap(a[i], a[p]);
        }
    }
 
    swap(a[p], a[first]);
 
    return p;
}

/**
* @brief Quicksort.
* @param a The array to be sorted.
* @param first The start of the sequence to be sorted.
* @param last The end of the sequence to be sorted.
*/
void quickSort(vector<string>& a, int first, int last)
{
    int pivotElement;
 
    if(first < last)
    {
        pivotElement = pivot(a, first, last);
        quickSort(a, first, pivotElement-1);
        quickSort(a, pivotElement+1, last);
    }
}