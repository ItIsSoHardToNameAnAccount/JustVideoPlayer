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
	Framework/VideoPlayer/include/JVideoPlayerBase.h
	Framework/VideoPlayer/include/JVideoWidget.h
	GUI/MainWindow/include/VMainWindow.h
	GUI/VideoWindow/include/JVideoWindow.h
)

set(SOURCE_FILES
	CommonLibs/Log/src/Logger.cpp
	CommonLibs/Ini/src/SimpleIni.cpp
	Framework/VideoPlayer/src/JVideoPlayerBase.cpp
	Framework/VideoPlayer/src/JVideoWidget.cpp
	GUI/MainWindow/src/VMainWindow.cpp
	GUI/VideoWindow/src/JVideoWindow.cpp
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
	${CMAKE_CURRENT_SOURCE_DIR}/GUI/VideoWindow/include
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/External/VLC/plugins
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins
)