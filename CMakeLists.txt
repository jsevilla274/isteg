cmake_minimum_required(VERSION 3.10)
project(isteg C)

set(CMAKE_C_STANDARD 11)

add_executable(isteg main.c bitmap.c bitmap.h util.h util.c stego.c stego.h)
