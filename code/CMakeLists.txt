cmake_minimum_required(VERSION 3.25)
project(project2)

set(CMAKE_CXX_STANDARD 14)

link_libraries(fltk)
link_libraries(fltk_images)

add_executable(project2 WIN32 main.cpp five_chess.rc)
