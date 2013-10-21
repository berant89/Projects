#include <iostream>
#include <sstream>

#include "FileManip.h"
#include "quicksort.h"

using namespace std;

//Global Variables
vector<string> listFiles;
string extension = "";
string newname = "";
int starting_num = 0;

void display_menu()
{
	cout<<"\n1) File extension.";
	cout<<"\n2) Basename (if you want just to number the files then leave this blank).";
	cout<<"\n3) Starting number.";
	cout<<"\n4) List files in current directory.";
	cout<<"\n5) Show new name.";
	cout<<"\n6) Rename the files.";
	cout<<"\n7) Exit the program.";
}

bool run_operation(int val)
{
	bool loop = true;
	switch(val)
	{
	case 1:
	{
		cout<<"\nGive the file extension (ex: .jpg):"<<endl;
		getline(cin, extension);
		ostringstream temp;
		temp<<"*"<<extension;
		listFiles = listFilesInDirectory(temp.str());
		//Sort the files using human sorting.
		quickSort(listFiles, 0, listFiles.size()-1);
		break;
	}
	case 2:
	{
		cout<<"\nType the new name:"<<endl;
		getline(cin, newname);
		break;
	}
	case 3:
	{
		cout<<"\nType the starting number:"<<endl;
		cin>>starting_num;
		cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
	case 4:
	{
		cout<<endl;
		print_names(listFiles);
		break;
	}
	case 5:
	{
		ostringstream temp;
		temp<<newname<<starting_num<<extension;
		cout<<"\nThe new name is: "<<temp.str()<<endl;
		break;
	}
	case 6:
	{
		rename_files(listFiles, newname, starting_num, extension);
		ostringstream temp;
		temp<<"*"<<extension;
		listFiles = listFilesInDirectory(temp.str());
		quickSort(listFiles, 0, listFiles.size()-1);
		break;
	}
	case 7:
	{
		loop = false;
		break;
	}
	default:
	{
		break;
	}
	}
	return loop;
}

int cmdUI()
{
	bool loop = true;
	cout<<"\n Welcome to the change filename program.\nDisplaying main menu:"<<endl;
	while(loop)
	{
		display_menu();
		cout<<"\nInput: ";
		int val;
		cin>>val;
		cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
		if(val>0 && val < 8)
			loop = run_operation(val);
		else
			cout<<"\nPlease type a valid number"<<endl;
	}
	return 0;
}

int main()
{
 //   listFiles = listFilesInDirectory("*.jpg");

	////Sort the files using human sorting.
	//quickSort(listFiles, 0, listFiles.size()-1);

	//print_names(listFiles);

	//string response = "";
	//cout<<"\nType in a name for the files? (y/n): ";
	//cin>>response;

	//if(!response.compare("y") || !response.compare("Y"))
	//{
	//	cout<<"\nType the base name that will be incremented then hit enter (don't type .jpg):"<<endl;
	//	cin>>newname;
	//}

	//rename_files(listFiles, newname);

	//exit();
	cmdUI();
	return 0;
}