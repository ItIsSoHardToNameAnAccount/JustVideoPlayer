#Set CMake minimum version
cmake_minimum_required( VERSION 3.15 )

#Set project name and version
project(JustVideoPlayer VERSION 0.0.1 LANGUAGES CXX)

#Use C++20
set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 COMPONENTS Gui Core Multimedia MultimediaWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#Official code, don't know why is needed
if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(HEADER_FILES
	Framework/VideoPlayer/include/VButtonArea.h
	Framework/VideoPlayer/include/VideoSlider.h
	Framework/VideoPlayer/include/VPlayList.h
	GUI/MainWindow/include/VMainWindow.h
)

set(SOURCE_FILES
	CommonLibs/Log/src/Logger.cpp
	CommonLibs/Ini/src/SimpleIni.cpp
	Framework/VideoPlayer/src/VButtonArea.cpp
	Framework/VideoPlayer/src/VideoSlider.cpp
	Framework/VideoPlayer/src/VPlayList.cpp
	GUI/MainWindow/src/VMainWindow.cpp
	JGlobal/JFile/src/JPlayListData.cpp
	JGlobal/vlcPlayer/src/vlcPlayer.cpp
	JGlobal/Volume/src/JVolume.cpp
)

link_directories(${CMAKE_SOURCE_DIR}/External/VLC/lib)

#Set executable file
add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CommonLibs/Log/include
	${CMAKE_CURRENT_SOURCE_DIR}/CommonLibs/Ini/include
	${CMAKE_CURRENT_SOURCE_DIR}/External/VLC/include
	${CMAKE_CURRENT_SOURCE_DIR}/Framework/VideoPlayer/include
	${CMAKE_CURRENT_SOURCE_DIR}/GUI/MainWindow/include
	${CMAKE_CURRENT_SOURCE_DIR}/JGlobal/JFile/include
	${CMAKE_CURRENT_SOURCE_DIR}/JGlobal/vlcPlayer/include
	${CMAKE_CURRENT_SOURCE_DIR}/JGlobal/Volume/include
)

target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Multimedia Qt5::MultimediaWidgets)
target_link_libraries(${PROJECT_NAME} libvlc libvlccore)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/External/VLC/lib/libvlc.dll
	${CMAKE_SOURCE_DIR}/External/VLC/lib/libvlccore.dll
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

set(VLC_PLUGIN_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins")
if(EXISTS ${VLC_PLUGIN_DIR})
	message(STATUS "Directory exists: ${VLC_PLUGIN_DIR}")
else()
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/External/VLC/plugins
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins
	)
endif()

# ToDo: Need to create Resources folder first
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/Resources/mainBackground.png
	${CMAKE_SOURCE_DIR}/Resources/playList.png
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)