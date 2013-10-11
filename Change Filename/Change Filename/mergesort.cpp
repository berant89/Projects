#include "mergesort.h"
#include "alphanum2.h"

#include <iostream>
#include <vector>
 
using namespace std;
 

void mergesort(vector<string>& s1, vector<string>& s2, int low, int high)
{
    int pivot;
    if(low<high)
    {
        pivot=(low+high)/2;
        mergesort(s1, s2, low, pivot);
        mergesort(s1, s2, pivot+1, high);
        merge(s1, s2, low, pivot, high);
    }
}

void merge(vector<string>& s1, vector<string>& s2, int low, int pivot, int high)
{
    int h,i,j,k;
    h=low;
    i=low;
    j=pivot+1;
 
    while((h<=pivot)&&(j<=high))
    {
        if(compare(s1[h], s1[j]))
        {
            s2[i]=s1[h];
            h++;
        }
        else
        {
            s2[i]=s1[j];
            j++;
        }
        i++;
    }
    if(h>pivot)
    {
        for(k=j; k<=high; k++)
        {
            s2[i]=s1[k];
            i++;
        }
    }
    else
    {
        for(k=h; k<=pivot; k++)
        {
            s2[i]=s1[k];
            i++;
        }
    }
    for(k=low; k<=high; k++) s1[k]=s2[k];
}