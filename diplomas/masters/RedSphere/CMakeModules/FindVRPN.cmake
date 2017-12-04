# This module defines

# VRPN_LIBRARY
# OSGVRPN_LIBRARY 
# VRPN_FOUND, if false, do not try to link to VRPN
# OSGVRPN_FOUND, if false, do not try to link to osgVRPN
# VRPN_INCLUDE_DIR, where to find the headers
# OSGVRPN_INCLUDE_DIR, where to find the headers

# to use this module, set variables to point to the osgEarth install directory
# VRPNDIR or VRPN_DIR
# OSGVRPNDIR OR OSGVRPN_DIR

FIND_PATH(VRPN_INCLUDE_DIR "vrpn_Configure.h"
	HINTS
		$ENV{VRPN_DIR}
		$ENV{VRPNDIR}
	PATH_SUFFIXES include
)

FIND_LIBRARY(VRPN_LIBRARY "vrpn"
    HINTS
		$ENV{VRPN_DIR}
		$ENV{VRPNDIR}
	PATH_SUFFIXES
		lib
		lib64
)

FIND_LIBRARY(VRPN_LIBRARY_DEBUG "vrpnd"
    HINTS
		$ENV{VRPN_DIR}
		$ENV{VRPNDIR}
	PATH_SUFFIXES
		lib
		lib64
)

FIND_PATH(OSGVRPN_INCLUDE_DIR "osgVRPN/Tracker.h"
	HINTS
		$ENV{OSGVRPN_DIR}
		$ENV{OSGVRPNDIR}
	PATH_SUFFIXES include
)

FIND_LIBRARY(OSGVRPN_LIBRARY "osgVRPN"
    HINTS
		$ENV{OSGVRPN_DIR}
		$ENV{OSGVRPNDIR}
	PATH_SUFFIXES
		lib
		lib64
)

FIND_LIBRARY(OSGVRPN_LIBRARY_DEBUG "osgVRPNd"
    HINTS
		$ENV{OSGVRPN_DIR}
		$ENV{OSGVRPNDIR}
	PATH_SUFFIXES
		lib
		lib64
)

SET(VRPN_FOUND "NO")
IF(VRPN_LIBRARY AND VRPN_INCLUDE_DIR)
  SET(VRPN_FOUND "YES")
  IF(VRPN_LIBRARY_DEBUG)
	SET(VRPN_LIBRARY
		optimized ${VRPN_LIBRARY} debug ${VRPN_LIBRARY_DEBUG}		
	)
  ELSE(VRPN_LIBRARY_DEBUG)
	
  ENDIF(VRPN_LIBRARY_DEBUG)  
ENDIF(VRPN_LIBRARY AND VRPN_INCLUDE_DIR)


SET(OSGVRPN_FOUND "NO")
IF(OSGVRPN_LIBRARY AND OSGVRPN_INCLUDE_DIR)
  SET(OSGVRPN_FOUND "YES")
  IF(OSGVRPN_LIBRARY_DEBUG)
	SET(OSGVRPN_LIBRARY
		optimized ${OSGVRPN_LIBRARY} debug ${OSGVRPN_LIBRARY_DEBUG}		
	)
  ELSE(OSGVRPN_LIBRARY_DEBUG)
	
  ENDIF(OSGVRPN_LIBRARY_DEBUG)  
ENDIF(OSGVRPN_LIBRARY AND OSGVRPN_INCLUDE_DIR)
