#include "UI.h"
#include "FileManip.h"
#include "quicksort.h"

#include <iostream>
#include <sstream>
#include <locale>

using namespace std;

/**
 * @brief The UI constructor that initializes the private variables and runs the UI.
 */
UI::UI(void)
{
	newname = "";
	extension = "";
	starting_num = 0;
	cmdUI();
}

/**
 * @brief Checks if the string is a digit.
 * @param s The string to be checked.
 * @return Wether the string is an integer or not.
 */
bool isNumber(const string& s)
{
	string::const_iterator it = s.begin();
	while(it != s.end() && isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

/**
 * @brief Displays the menu to the command prompt.
 */
void UI::display_menu()
{
	cout<<"\n1) File extension.";
	cout<<"\n2) Basename (if you want just to number the files then leave this blank).";
	cout<<"\n3) Starting number.";
	cout<<"\n4) List files in current directory.";
	cout<<"\n5) Show new name.";
	cout<<"\n6) Rename the files.";
	cout<<"\n7) Exit the program.";
}

/**
 * @brief Main loop of the command prompt UI.
 * @return Returns 0 to indicate successful execution.
 */
int UI::cmdUI()
{
	bool loop = true; //To keep the loop running
	int val = -1; //Menu value
	string input = ""; //Input from the user.
	cout<<"\n Welcome to the change filename program.\nDisplaying main menu:"<<endl;
	while(loop)
	{
		display_menu();
		//Record the input.
		cout<<"\nInput: ";
		getline(cin, input);

		//Check if the input is a number
		if(isNumber(input))
		{
			//Safely converts a string to a number.
			stringstream stream(input);
			stream>>val;
		}
		else
			val = -1;
		
		//Check if the value is within its limits and the string is a single character.
		if(val > 0 && val < 8)
			loop = run_operation(val);
		else
			cout<<"\nPlease type a valid number from the menu."<<endl;
	}
	return 0;
}

/**
 * @brief Runs the given operation from the menu.
 * @param val The option chosen from the menu.
 * @return Wether to continue looping or not.
 */
bool UI::run_operation(int val)
{
	bool loop = true; //Indicates if the loops need to keep running.
	switch(val)
	{
	case 1:
	{
		cout<<"\nGive the file extension (ex: .jpg):"<<endl;
		getline(cin, extension);
		//Format for obtaining the files in the directory of the executable.
		ostringstream temp;
		temp<<"*"<<extension;
		//Get the files that have the same exentsion.
		listFiles = listFilesInDirectory(temp.str());
		//Sort the files using human sorting.
		quickSort(listFiles, 0, listFiles.size()-1);
		break;
	}
	case 2:
	{
		cout<<"\nType the new name (hit enter if you want this to be blank):"<<endl;
		getline(cin, newname);
		break;
	}
	case 3:
	{
		string input = "";
		int temp = 0;
		cout<<"\nType the starting number:"<<endl;
		getline(cin, input);
		//Check if the input is a number.
		if(isNumber(input))
		{
			//Safely converts a string to a number.
			stringstream stream(input);
			stream>>temp;
			starting_num = temp;
			break;
		}
		cout<<"\nPlease enter a valid number."<<endl;
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
		//Get the new list of files.
		listFiles = rename_files(listFiles, newname, starting_num, extension);
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