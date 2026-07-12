cmake_minimum_required(VERSION 3.18)

project(Tetrix)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
find_library(SDL2_TTF_LIBRARY SDL2_ttf)
include_directories(${SDL2_TTF_INCLUDE_DIRS})
find_library(LOG4C_LIBRARY NAMES log4c)
include_directories(${LOG4C_INCLUDE_DIRS})
find_library(CHECK_LIBRARY NAMES check)
include_directories(${CHECK_INCLUDE_DIRS})
include_directories(inc/)

set(SOURCES src/main.cpp
            src/gamefield.cpp
            src/figure.cpp
            src/draw.cpp
            src/game.cpp
            src/init.cpp)

set(INCLUDE inc/global.h
            inc/gamefield.h
            inc/figure.h
            inc/draw.h
            inc/types.h
            inc/game.h)



add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
                      ${SDL2_LIBRARIES} 
                      ${SDL2_IMAGE_LIBRARIES} 
                      ${SDL2_TTF_LIBRARY} 
                      ${LOG4C_LIBRARY} 
                      ${CHECK_LIBRARY})

if(UNIX)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif (UNIX)

