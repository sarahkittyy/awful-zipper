cmake_minimum_required(VERSION 3.13)
project(awful-zipper)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources "src/*.cpp")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")

macro(setopt opt value)
	option(${opt} "" ${value})
endmacro()
setopt(BUILD_TEST OFF)
setopt(BUILD_SHARED_LIBS ON)
setopt(BUILD_STATIC_VERSION OFF)
setopt(WITH_WALL OFF)

add_definitions(-DHAVE_AES)

add_subdirectory(lib/imgui-sfml)
add_subdirectory(lib/zipper)

add_executable(awful-zipper ${sources})

target_include_directories(awful-zipper PUBLIC
	"include/"
	"lib/imgui"
	"lib/imgui-sfml"
	"lib/imgui-filebrowser"
	"lib/zipper/zipper"
)
target_compile_options(awful-zipper PUBLIC -Wall)
target_link_libraries(awful-zipper Zipper ZLIB::ZLIB pthread sfml-graphics sfml-window sfml-system ImGui-SFML::ImGui-SFML)