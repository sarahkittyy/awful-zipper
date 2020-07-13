#include <zipper.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "compress.hpp"

namespace compress {

void to_zip(std::set<std::string> paths, std::string output) {
	namespace fs = std::filesystem;

	if (fs::exists(output)) {
		fs::remove(output);
	}

	zipper::Zipper z(output);

	for (auto& name : paths) {
		fs::path p(name);
		z.add(p.string(), z.Overwrite);
	}

	z.close();
}

void create_file(std::string path, std::string content) {
	std::ofstream f(path);
	f << content;
	f.close();
}

std::string random_str() {
	std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@!#$%";
	int len			  = rand() % 15 + 7;

	std::string res;

	for (int i = 0; i < len; ++i) {
		res.push_back(chars[rand() % chars.length()]);
	}

	return res;
}

std::string random_path_str() {
	std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	int len			  = rand() % 15 + 7;

	std::string res;

	for (int i = 0; i < len; ++i) {
		res.push_back(chars[rand() % chars.length()]);
	}

	return res;
}

std::string to_passworded_zip(std::set<std::string> paths, std::string output) {
	namespace fs = std::filesystem;

	std::string random_pass = random_str();
	;
	std::vector<std::string> potential_passwords;
	int ct = rand() % 4 + 5;
	for (int i = 0; i < ct; ++i) {
		potential_passwords.push_back(random_str());
	}

	if (fs::exists(output)) {
		fs::remove(output);
	}

	zipper::Zipper z(output, random_pass);

	for (auto& name : paths) {
		fs::path p(name);
		z.add(p.string(), z.Overwrite);
	}

	z.close();

	std::string final_res = "it's one of these! <3\n";
	potential_passwords.push_back(random_pass);
	std::random_shuffle(potential_passwords.begin(), potential_passwords.end());
	for (auto& s : potential_passwords) {
		final_res += s + "\n";
	}

	return final_res;
}

void create_tons_of_dirs(std::set<std::string> files, std::string final) {
	namespace fs = std::filesystem;

	fs::create_directory(final);

	fs::path p(final);

	p /= random_path_str();

	for (int i = 0; i < 10; ++i) {
		fs::create_directory(p);
		p /= random_path_str();
	}

	for (auto& f : files) {
		fs::path from(f);
		fs::path to(p.parent_path());
		to /= from.filename();
		fs::copy(from, to, fs::copy_options::recursive);
	}
}

}