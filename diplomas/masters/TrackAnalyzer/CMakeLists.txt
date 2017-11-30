CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(TrackAnalyzer)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_DEBUG_POSTFIX "d")

SET(HEADER_FILES
    headers/gnuplot_iostream.h
    headers/track_params.h
    headers/gplotter.h
    headers/helpers.h
    headers/track_analyzer.h
)

SET(SRC_FILES	
    sources/gplotter.cpp
    sources/helpers.cpp
    sources/track_analyzer.cpp
    sources/track_params.cpp
    sources/main.cpp
)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(Boost 1.55.0 COMPONENTS iostreams filesystem system REQUIRED)
FIND_PACKAGE(Qt4 4.8)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDES} headers)

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
 
ADD_EXECUTABLE(TrackAnalyzer ${SRC_FILES} ${HEADER_FILES} ${LIB_UI_HDRS})

TARGET_LINK_LIBRARIES(
    TrackAnalyzer
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTMAIN_LIBRARY}
    ${Boost_LIBRARIES}
)

INSTALL(TARGETS TrackAnalyzer RUNTIME DESTINATION bin)
