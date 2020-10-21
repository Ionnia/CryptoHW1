cmake_minimum_required(VERSION 3.17)
project(CryptoHW1)
set(CMAKE_CXX_STANDARD 14)

add_executable(CryptoHW1 src/main.cpp src/Feistel.h src/Feistel.cpp src/Utils.h src/Utils.cpp)