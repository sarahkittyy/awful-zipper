#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <set>

#include <thread>

#include "background.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include "imfilebrowser.h"

class app {
public:
	app();

	int run();

private:
	sf::RenderWindow window;
	sf::Clock clock;

	background bg;
	ImGui::FileBrowser fb;
	ImGui::FileBrowser db;

	void draw_gui();

	void execute_awful_zip();

	std::set<std::string> files;

	char output[50];
	std::string output_string() const;

	bool compressing;
	std::vector<std::string> compressionLog;
	void clog(std::string s);

	std::thread zipping_thread;
};