#pragma once

#include <vector>
#include <string>

class UI
{
private:
	std::vector<std::string> listFiles;
	std::string newname;
	std::string extension;
	int starting_num;

	void display_menu();
	bool run_operation(int val);
	int cmdUI();

public:
	UI(void);
};

