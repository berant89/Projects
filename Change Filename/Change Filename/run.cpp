#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>

#include "quicksort.h"

using namespace std;

string wchar_t2string(const wchar_t *wchar)
{
    string str = "";
    int index = 0;
    while(wchar[index] != 0)
    {
        str += (char)wchar[index];
        ++index;
    }
    return str;
}

wchar_t *string2wchar_t(const string &str)
{
    wchar_t wchar[260];
    int index = 0;
    while(index < str.size())
    {
        wchar[index] = (wchar_t)str[index];
        ++index;
    }
    wchar[index] = 0;
    return wchar;
}

vector<string> listFilesInDirectory(string directoryName)
{
    WIN32_FIND_DATA FindFileData;
    wchar_t * FileName = string2wchar_t(directoryName);
    HANDLE hFind = FindFirstFile(FileName, &FindFileData);

    vector<string> listFileNames;
    listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

    while (FindNextFile(hFind, &FindFileData))
        listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

    return listFileNames;
}

void print_names(vector<string>& listFiles)
{
	for each (string str in listFiles)
        cout << str << endl;
}
void exit();
void rename_files(vector<string>& listFiles, string newname)
{
	int result = -1;
	int number = 1;
	for each(string oldname in listFiles)
	{
		ostringstream filename;
		filename<<newname<<number<<".jpg";

		result = rename(oldname.c_str(), filename.str().c_str());
		if(result == 0)
			cout<<"Successfully renamed: "<<oldname<<" to "<<filename.str()<<endl;
		else
		{
			cout<<"FAILD to rename: "<<oldname<<" to "<<filename.str()<<endl;
			exit();
			exit(1);
		}
		number++;
	}
}

void exit()
{
	string a = "";
	while(a.compare("exit"))
	{
		cout<<"\nType 'exit' then hit enter to quit."<<endl;
		cin>>a;
	}
}

int main()
{
	vector<string> listFiles;

    listFiles = listFilesInDirectory("*.jpg");
	
	cout<<endl;
	quickSort(listFiles, 0, listFiles.size()-1);

	print_names(listFiles);

	string newname = "";

	string response = "";
	cout<<"\nType in a name for the files? (y/n): ";
	cin>>response;

	if(!response.compare("y") || !response.compare("Y"))
	{
		cout<<"\nType the base name that will be incremented then hit enter (don't type .jpg): ";
		cin>>newname;
	}

	rename_files(listFiles, newname);

	exit();
	return 0;
}