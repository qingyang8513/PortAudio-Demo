# CMake version minimum required.
cmake_minimum_required (VERSION 2.8)

# Project name, change it you need.
SET(PRJ_NAME parecord)
project (${PRJ_NAME})

# Combine source files to DIR_SRCS in current forld.
aux_source_directory(. DIR_SRCS)

# add sub forld
#add_subdirectory(include)

# add include dir
include_directories(../include)

# add libs dir
link_directories("../lib")

#link_libraries("./lib/libportaudio.a")

# Specify name to be build.
add_executable(${PRJ_NAME} ${DIR_SRCS})

# Link libs
target_link_libraries(${PRJ_NAME} portaudio)
target_link_libraries(${PRJ_NAME} m)
target_link_libraries(${PRJ_NAME} asound)
target_link_libraries(${PRJ_NAME} pthread)
