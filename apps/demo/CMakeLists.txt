add_executable(demo
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet_breathy_runner.cpp
)

find_package(OpenCV REQUIRED)
find_package(CLI11 REQUIRED)

target_include_directories(demo
    PUBLIC
        ${PetBreathyLib}/libs/pet-breathy/include
        ${OpenCV_INCLUDE_DIRS}
        ${CLI11_INCLUDE_DIRS}
)

target_link_libraries(demo
    PUBLIC
        petbreathy
        ${OpenCV_LIBS}
)
