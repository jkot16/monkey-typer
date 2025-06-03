cmake_minimum_required(VERSION 3.27)
project(Project)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(fmt)
add_executable(Project main.cpp
        Menu.h
        Menu.cpp
        ScoreBar.h
        ScoreBar.cpp
        FileManager.h
        FileManager.cpp)
target_link_libraries(Project
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
)
target_link_libraries(Project fmt)
