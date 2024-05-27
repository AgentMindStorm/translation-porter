cmake_minimum_required(VERSION 3.26)
project(translation_porter)

set(CMAKE_CXX_STANDARD 17)

add_executable(translation_porter main.cpp)

configure_file(colors_classic.txt colors_classic.txt COPYONLY)
configure_file(colors_new.txt colors_new.txt COPYONLY)
configure_file(languages.txt languages.txt COPYONLY)
configure_file(multiple.txt multiple.txt COPYONLY)
