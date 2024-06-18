#Set CMake minimum version
cmake_minimum_required( VERSION 3.15 )

#Set project name and version
project(CMakeTemplate VERSION 0.0.0 LANGUAGES CXX)

#Use C++20
set(CMAKE_CXX_STANDARD 20)

#Official code, don't know why is needed
if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(HEADER_FILES
	# Only when needed
)

set(SOURCE_FILES
	CommonLibs/Log/src/Logger.cpp
)

#Set executable file
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CommonLibs/Log/include
)

target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)