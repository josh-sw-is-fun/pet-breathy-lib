cmake_minimum_required(VERSION 3.10)
project(PetBreathyLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include(CTest)
enable_testing()

if (CMAKE_TESTING_ENABLED)
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

add_subdirectory(libs/pet-breathy)
add_subdirectory(apps/demo)
