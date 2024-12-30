cmake_minimum_required(VERSION 3.12)

add_library(Froth 
  core/Entrypoint.h
  core/Entrypoint.cpp
  core/Application.h
  core/Application.cpp
)

target_include_directories(Froth PRIVATE ../froth)
