cmake_minimum_required(VERSION 3.5)
project(Minesweeper VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(${PROJECT_SOURCE_DIR}/src)

include(CTest)
enable_testing()

add_executable(Minesweeper 
    src/main.cpp
    src/minefield.cpp
    src/cell.cpp
    src/playerinput.cpp)

# add_subdirectory(${PROJECT_SOURCE_DIR}/src)

set_property(TARGET Minesweeper PROPERTY CXX_STANDARD 23)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

