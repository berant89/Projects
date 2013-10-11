#include <iostream>

#include "FileManip.h"
#include "quicksort.h"

using namespace std;

int main()
{
	vector<string> listFiles;
    listFiles = listFilesInDirectory("*.jpg");

	//Sort the files using human sorting.
	quickSort(listFiles, 0, listFiles.size()-1);

	print_names(listFiles);

	string newname = "";

	string response = "";
	cout<<"\nType in a name for the files? (y/n): ";
	cin>>response;

	if(!response.compare("y") || !response.compare("Y"))
	{
		cout<<"\nType the base name that will be incremented then hit enter (don't type .jpg):"<<endl;
		cin>>newname;
	}

	rename_files(listFiles, newname);

	exit();
	return 0;
}