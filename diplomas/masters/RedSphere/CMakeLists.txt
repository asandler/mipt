CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(RedSphere)

SET(CMAKE_MODULE_PATH "${RedSphere_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
FIND_PACKAGE(VRPN)


FIND_PACKAGE(Qt4 4.6)
IF (QT4_FOUND)
    INCLUDE(${QT_USE_FILE})
    SET(QT_ALL_LIBRARIES ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTOPENGL_LIBRARY})
ENDIF (QT4_FOUND)

OPTION(USE_QT "Enable to use Qt (build Qt-dependent libraries, plugins and examples)" ON)

FIND_PACKAGE(OpenSceneGraph 2.8.2 REQUIRED
	osg 
	osgUtil
	osgViewer
	osgText	
	osgDB
	osgGA
	osgQt
	osgShadow
	osgParticle
)

SET(SRC_FILES
	${RedSphere_SOURCE_DIR}/Manipulator.cpp
	${RedSphere_SOURCE_DIR}/Program.cpp
)

SET(HEADER_FILES
	${RedSphere_SOURCE_DIR}/Manipulator.h
)

IF (VRPN_FOUND AND OSGVRPN_FOUND)
	ADD_DEFINITIONS(-DUSE_VRPN)
	INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}
		${VRPN_INCLUDE_DIR}
		${OSGVRPN_INCLUDE_DIR}		
	)
ENDIF (VRPN_FOUND AND OSGVRPN_FOUND)

SET(CMAKE_INCLUDE_PATH CACHE STRING "Put your OSG include dir here...")
SET(CMAKE_LIBRARY_PATH CACHE STRING "Put your OSG lib dir here...")

SET(CMAKE_DEBUG_POSTFIX "d")

# Stuff that should be in every build...
INCLUDE_DIRECTORIES(
	${RedSphere_SOURCE_DIR}
	${CMAKE_INCLUDE_PATH}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
	${RedSphere_SOURCE_DIR}/3rdParty/lib
	${CMAKE_LIBRARY_PATH}
)

################################
ADD_EXECUTABLE(RedSphere ${SRC_FILES} ${HEADER_FILES})

TARGET_LINK_LIBRARIES(RedSphere ${OPENSCENEGRAPH_LIBRARIES})

IF (VRPN_FOUND AND OSGVRPN_FOUND)
	TARGET_LINK_LIBRARIES(RedSphere ${VRPN_LIBRARY} ${OSGVRPN_LIBRARY})
ENDIF (VRPN_FOUND AND OSGVRPN_FOUND)

IF(WIN32)
	TARGET_LINK_LIBRARIES(RedSphere opengl32.lib)
ENDIF(WIN32)

INSTALL(TARGETS RedSphere RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
