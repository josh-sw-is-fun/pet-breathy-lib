
find_package(OpenCV REQUIRED)

add_library(petbreathy STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/optical_flow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/patch_analyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/peak_finding.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pet_breathy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point_budget_calculator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point_group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point_group_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/point_monitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_analyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/video_display.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/video_reader.cpp
)

target_compile_options(petbreathy
    PRIVATE
        -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(petbreathy
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(petbreathy
    PUBLIC
        ${OpenCV_LIBS}
)

if (CMAKE_TESTING_ENABLED)
    add_executable(test_petbreathy
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_peak_finding.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_pet_breathy.cpp
    )
    
    target_include_directories(test_petbreathy
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(test_petbreathy
        PUBLIC
            petbreathy
            GTest::gtest_main
    )
endif()
