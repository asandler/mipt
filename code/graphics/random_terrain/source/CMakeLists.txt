CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME dodecahedron)

PROJECT(${PROJECT_NAME})

FUNCTION(MY_FIND_PATH module filename)
    FIND_PATH(${module} ${filename}
        PATH_SUFFIXES include
        PATHS
            /opt
            /usr
            /usr/local
    )
ENDFUNCTION(MY_FIND_PATH module filename)

FUNCTION(MY_FIND_LIBRARY module filename)
    FIND_LIBRARY(${module} ${filename}
        PATH_SUFFIXES
            lib
            lib64
        PATHS
            /opt
            /usr
            /usr/local
    )
ENDFUNCTION(MY_FIND_LIBRARY module filename)

MY_FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h)
MY_FIND_PATH(GLFW_INCLUDE_DIR GLFW/glfw3.h)
MY_FIND_PATH(GLIMAGE_INCLUDE_DIR glimg/glimg.h)
MY_FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp)
MY_FIND_PATH(BOOST_INCLUDE_DIR boost/smart_ptr/shared_ptr.hpp)

MY_FIND_LIBRARY(GLEW_LIBRARY GLEW)
MY_FIND_LIBRARY(GLFW_LIBRARY glfw3)
MY_FIND_LIBRARY(GLIMAGE_LIBRARY glimg)

IF (NOT WIN32)
    FIND_PACKAGE(X11 REQUIRED)
    FIND_PACKAGE(OpenGL REQUIRED)
    ADD_DEFINITIONS(${OpenGL_DEFINITIONS})
    IF(NOT OPENGL_FOUND)
        MESSAGE(ERROR " OPENGL not found!")
    ENDIF(NOT OPENGL_FOUND)
    MY_FIND_LIBRARY(PTHREAD_LIBRARY pthread)
ENDIF (NOT WIN32)

INCLUDE_DIRECTORIES(
    ${CMAKE_INCLUDE_PATH}
    ${OpenGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}	
    ${GLIMAGE_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
)

LINK_DIRECTORIES(
    ${CMAKE_LIBRARY_PATH}
    ${OpenGL_LIBRARY_DIRS}
)

SET(SRC_FILES
    application.cpp
    camera.cpp
    common_material.cpp
    mesh.cpp
    program.cpp
    material.cpp
    navigation.cpp
    texture.cpp
)

SET(HEADER_FILES
    application.h
    camera.h
    common_material.h
    material.h
    mesh.h
    texture.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GLEW_LIBRARY} ${GLFW_LIBRARY} ${GLIMAGE_LIBRARY})

IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} opengl32.lib)
ELSE(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${X11_Xxf86vm_LIB} ${X11_Xi_LIB} ${X11_Xrandr_LIB} ${PTHREAD_LIBRARY})
ENDIF(WIN32)

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../install)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/../resources/shaders/ DESTINATION ${CMAKE_SOURCE_DIR}/../install/shaders)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/../resources/images/ DESTINATION ${CMAKE_SOURCE_DIR}/../install/images)
