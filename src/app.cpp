#include <cstdlib>
#include <ctime>

#include "app.hpp"
#include "compress.hpp"

app::app()
	: window(sf::VideoMode(800, 600), "awful-zipper"),
	  db(ImGuiFileBrowserFlags_SelectDirectory),
	  output{ 0 },
	  compressing(false) {
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);

	fb.SetPwd(std::filesystem::current_path());
	fb.SetTitle("pick a file to add...");

	db.SetPwd(std::filesystem::current_path());
	db.SetTitle("pick a directory to add...");
}

int app::run() {
	while (window.isOpen()) {
		// handle events
		sf::Event e;
		while (window.pollEvent(e)) {
			ImGui::SFML::ProcessEvent(e);

			switch (e.type) {
			default:
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		// update
		ImGui::SFML::Update(window, clock.restart());

		// draw the gui
		draw_gui();

		// draw everything
		window.clear(sf::Color::White);
		window.draw(bg);

		ImGui::SFML::Render(window);
		window.display();
	}

	if (zipping_thread.joinable()) {
		zipping_thread.join();
	}

	ImGui::SFML::Shutdown();

	return 0;
}

void app::draw_gui() {
	ImGui::Begin("awful-zipper");

	ImGui::Text("%lu file(s)", files.size());

	for (auto i = files.begin(); i != files.end();) {
		ImGui::PushID(std::distance(files.begin(), i));
		ImGui::Text("%s", i->c_str());
		ImGui::SameLine();
		if (ImGui::Button("X")) {
			i = files.erase(i);
		} else {
			++i;
		}
		ImGui::PopID();
	}

	if (ImGui::Button("add file")) {
		fb.Open();
	}
	if (ImGui::Button("add directory")) {
		db.Open();
	}
	ImGui::Text("output file (extension automatically added):");
	ImGui::InputText("", output, 49);
	ImGui::SameLine();

	if (ImGui::Button("compress! <3")) {
		if (output_string().empty()) {
			ImGui::OpenPopup("error popup");
		} else {
			zipping_thread = std::thread([this]() {
				execute_awful_zip();
			});
		}
	}

	if (ImGui::BeginPopup("error popup")) {
		ImGui::Text("cannot have empty file name :(");
		ImGui::EndPopup();
	}

	ImGui::End();

	fb.Display();
	db.Display();

	if (fb.HasSelected()) {
		files.insert(fb.GetSelected().string());
		fb.ClearSelected();
	}
	if (db.HasSelected()) {
		files.insert(db.GetSelected().string());
		db.ClearSelected();
	}

	// render compression log if running
	if (compressing) {
		ImGui::SetNextWindowSize(ImVec2(500, 500));
		ImGui::Begin("running....");

		for (auto& line : compressionLog) {
			ImGui::Text("%s", line.c_str());
		}

		ImGui::SetScrollHereY();

		ImGui::End();
	}
}

void app::execute_awful_zip() {
	namespace fs = std::filesystem;

	const int STEPS = 3;

	compressing = true;
	clog("starting...");

	clog("clearing intermediate dir...");
	for (auto& f : fs::directory_iterator("intermediate/")) {
		if (f.is_directory()) {
			fs::remove_all(f);
		} else if (f.path().stem() != ".gitignore") {
			fs::remove(f);
		}
	}

	std::set<std::string> cfiles = files;
	std::set<std::string> used_paths;

	int iters = rand() % 8 + 16;

	for (int i = 0; i < iters; ++i) {
		std::string i_str;
		do {
			i_str = compress::random_path_str();
		} while (used_paths.count(i_str) != 0);
		used_paths.insert(i_str);
		std::string cpath = "intermediate/" + i_str;

		int step = rand() % STEPS;
		switch (step) {
		default:
			break;
		case 0:
			clog("compressing files into regular zip...");
			compress::to_zip(cfiles, cpath + ".zip");
			clog("compressed to " + cpath + ".zip");
			cfiles.clear();
			cfiles.insert(cpath + ".zip");
			break;
		case 1: {
			clog("compressing into random password zip");
			auto pass = compress::to_passworded_zip(cfiles, cpath + ".zip");
			clog("compressed to " + cpath + ".zip");
			clog("creating password file");
			compress::create_file("intermediate/passes.txt", pass);
			cfiles.clear();
			cfiles.insert(cpath + ".zip");
			cfiles.insert("intermediate/passes.txt");
			clog("created!");
			break;
		}
		case 2: {
			clog("creating a ton of nested directories...");
			compress::create_tons_of_dirs(cfiles, cpath);
			cfiles.clear();
			cfiles.insert(cpath);
			clog("new path is " + cpath);
			clog("did that <33");
			break;
		}
		}
	}
	clog("adding all to final zip...");
	compress::to_zip(cfiles, output_string() + ".zip");

	clog("done!");
}

std::string app::output_string() const {
	return std::string(output);
}

void app::clog(std::string s) {
	compressionLog.push_back(s);
}

int main() {
	srand(time(NULL));

	app a;
	return a.run();
}