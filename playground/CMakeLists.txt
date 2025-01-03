cmake_minimum_required(VERSION 3.12)

add_executable(Playground
  Playground.cpp
  ../froth/core/Entrypoint.h
)
target_link_libraries(Playground PUBLIC Froth)

