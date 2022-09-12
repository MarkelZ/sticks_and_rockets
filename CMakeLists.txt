cmake_minimum_required(VERSION 3.16.0)
project(PhysicsGame)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_subdirectory(game)
