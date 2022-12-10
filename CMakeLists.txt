cmake_minimum_required(VERSION 3.10)
project(cmake-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(. src)

add_executable(program ${src})
# target_compile_features(program PRIVATE cxx_std_20)
