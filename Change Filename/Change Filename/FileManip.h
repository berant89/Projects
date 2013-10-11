#pragma once

#include <string>
#include <vector>

std::vector<std::string> listFilesInDirectory(const std::string& directoryName);
void print_names(std::vector<std::string>& listFiles);
std::vector<std::string> rename_files(std::vector<std::string>& listFiles, const std::string& newname);
void exit();