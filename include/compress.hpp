#pragma once

#include <set>
#include <string>

namespace compress {

void to_zip(std::set<std::string> files, std::string output);
std::string to_passworded_zip(std::set<std::string> files, std::string output);

void create_file(std::string path, std::string content);

void create_tons_of_dirs(std::set<std::string> files, std::string final);

std::string random_path_str();
std::string random_str();

}