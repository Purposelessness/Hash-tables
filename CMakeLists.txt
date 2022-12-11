cmake_minimum_required(VERSION 3.10)
project(hash_tables)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(. src)

add_executable(hash_tables ${src} DoubleHashing.h ChainMethod.h Hash.cpp Hash.h)
# target_compile_features(program PRIVATE cxx_std_20)
