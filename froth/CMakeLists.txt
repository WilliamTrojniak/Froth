cmake_minimum_required(VERSION 3.12)

add_library(Froth 
  core/Application.h
  core/Application.cpp
  core/Entrypoint.h
  core/Entrypoint.cpp
)

target_include_directories(Froth PRIVATE ./)
