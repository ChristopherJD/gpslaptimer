############################## Project Version ################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)

SET(LSM9DS1_VERSION_MAJOR 0)
SET(LSM9DS1_VERSION_MINOR 1)
SET(LSM9DS1_VERSION_PATCH 0)
SET(BUILD_VERSION 
    ${LSM9DS1_VERSION_MAJOR}.${LSM9DS1_VERSION_MINOR}.${LSM9DS1_VERSION_PATCH})
###############################################################################

PROJECT(gpslaptimer)
SET(PROJECT "gpslaptimer")
SET(LICENSE "GPLV3")
###############################################################################

########################### Build and include settings ########################
STRING(CONCAT CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG=1 -g")
STRING(CONCAT CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -pedantic")

INCLUDE_DIRECTORIES(
    inc
)

LINK_DIRECTORIES(
    ${LINK_DIRECTORIES}
)

########################## C Library ##########################################
FILE(GLOB HEADERS
    "inc/*.h"
)
FILE(GLOB SOURCES 
    "src/*.c" 
)

ADD_EXECUTABLE(${PROJECT} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT} gps)

########################## C Library ##########################################
INSTALL(TARGETS ${PROJECT}
	DESTINATION bin
)
###############################################################################

######################## RPM Package ##########################################
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "raspberrypi3")
SET(CPACK_PACKAGE_VERSION ${BUILD_VERSION})
SET(CPACK_GENERATOR "RPM;TGZ")
SET(CPACK_PACKAGE_NAME ${PROJECT})
SET(CPACK_PACKAGE_RELEASE 1)
SET(CPACK_PACKAGE_CONTACT "Christopher Jordan-Denny")
SET(CPACK_PACKAGE_VENDOR "Christopher Jordan-Denny")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library for the LSM9DS1.")
SET(CPACK_RPM_PACKAGE_LICENSE ${LICENSE})
STRING(CONCAT CPACK_PACKAGE_FILE_NAME 
    "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}"
    "-${CPACK_RPM_PACKAGE_ARCHITECTURE}")
INCLUDE(CPack)
###############################################################################
