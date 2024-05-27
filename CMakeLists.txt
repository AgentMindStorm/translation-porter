cmake_minimum_required(VERSION 3.26)
project(Code_Projects)

set(CMAKE_CXX_STANDARD 17)

add_executable(Code_Projects main.cpp)

configure_file(colors.txt colors.txt COPYONLY)
configure_file(languages.txt languages.txt COPYONLY)
configure_file(multiple.txt multiple.txt COPYONLY)
