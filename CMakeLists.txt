cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT DarkStorm-Base-X64)
project(${PROJECT})

# Removed temporarily since it seems to not be working properly for some reason
#find_file(WINDOWS_HEADER windows.h)
#if(NOT WINDOWS_HEADER)
#  message(FATAL_ERROR "Can't find windows.h!")
#endif()

#add_link_options(
#  /DEBUG
#  /INCREMENTAL:NO
#)

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/dependencies/MinHook/*.c src/dependencies/MinHook/hde/*.c)
add_library(${PROJECT} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE ) # currently unused

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")

#add_subdirectory(libs/minhook)
#add_subdirectory(libs/imgui-hook)
#target_link_libraries(${PROJECT} minhook imgui-hook opengl32) # OpenGL32 for imgui

#target_link_libraries(${PROJECT} ${CMAKE_SOURCE_DIR}/libs/cocos-headers/cocos2dx/libcocos2d.lib)
#target_link_libraries(${PROJECT} ${CMAKE_SOURCE_DIR}/libs/cocos-headers/extensions/libExtensions.lib)
#target_link_libraries(${PROJECT} ${CMAKE_SOURCE_DIR}/libs/soloud/soloud_static.lib)
