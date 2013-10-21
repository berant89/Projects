#include "FileManip.h"

#include <iostream>
#include <sstream>
#include <Windows.h>

using namespace std;

/**
 * @brief Converts a wchar_t to a string.
 * @param wchar Pointer to the wchar_t.
 * @return A string representation.
 */
string wchar_t2string(const wchar_t* wchar)
{
    string str = "";
    for(int index = 0; wchar[index] != 0; ++index)
        str += (char)wchar[index];
    return str;
}

/**
 * @brief Converts a string to a wchar_t.
 * @param str Pointer to the string.
 * @return A string representation.
 */
wchar_t* string2wchar_t(const string& str)
{
    wchar_t wchar[260]; //Hardcode the size of wchar.
    int index = 0;
    for(int size = str.size(); index < size; ++index)
        wchar[index] = (wchar_t)str[index];
    wchar[index] = 0;
    return wchar;
}

/**
 * @brief Returns a list of files in the directory.
 * @param directoryName A string containing the path and possible a regexp for wanted files.
 * @return A pointer to the vector.
 */
vector<string> listFilesInDirectory(const string& directoryName)
{
    WIN32_FIND_DATA FindFileData;
	//Convert the string to a wchar_t
    wchar_t* FileName = string2wchar_t(directoryName);
    HANDLE hFind = FindFirstFile(FileName, &FindFileData);

	//Create vector that will hold list the filenames in ANSI sorting.
    vector<string> listFileNames;
	//Append to the list the converted wchar_T
    listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

	//Iterate through the list and append them.
    while (FindNextFile(hFind, &FindFileData))
        listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

    return listFileNames;
}

/**
 * @brief Prints out the content of the list.
 * @param listFiles Pointer to a vector of strings.
 */
void print_names(vector<string>& listFiles)
{
	for each(string str in listFiles)
        cout<<str<<endl;
}

/**
 * @brief Renames the files with the given basename, an incremented value and its extension.
 * @param listFiles The list of files to be renamed.
 * @param newname The basename.
 * @param number The starting number to increment from. By default it starts at 1.
 * @return A list of the new filenames.
 */
vector<string> rename_files(vector<string>& listFiles, const string& newname, int number = 1, const string& extension = ".jpg")
{
	vector<string> newFiles;
	int result = -1;
	for each(string oldname in listFiles)
	{
		ostringstream filename;
		filename<<newname<<number<<extension;
		string fstr = filename.str();

		result = rename(oldname.c_str(), fstr.c_str());
		if(result == 0)
			cout<<"Successfully renamed: "<<oldname<<" to "<<filename.str()<<endl;
		else
		{
			cout<<"FAILD to rename: "<<oldname<<" to "<<filename.str()<<endl;
			exit(); //Gives the user time to see which file failed.
			exit(1);
		}
		newFiles.push_back(fstr);
		number++;
	}
	return newFiles;
}

/**
 * @brief Prints an exit message.
 */
void exit()
{
	string a = "";
	while(a.compare("exit"))
	{
		cout<<"\nType 'exit' then hit enter to quit."<<endl;
		cin>>a;
	}
}