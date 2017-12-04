# This module defines

# OSGAUDIO_LIBRARY
# OPENALPP_LIBRARY
# OSGAUDIO_FOUND, if false, do not try to link to osgEarth
# OSGAUDIO_INCLUDE_DIRS, where to find the headers

# to use this module, set variables to point to the osgEarth install directory
# OSGEARTHDIR or OSGEARTH_DIR

SET(SOUND_MODULES_DIR CACHE STRING "")

FUNCTION(SOUND_FIND_LIBRARY module filename)
	FIND_PATH(${module} ${filename}
		HINTS
			$ENV{SOUND_MODULES_DIR}				
		PATH_SUFFIXES include/AL include
		PATHS
			/sw # Fink
			/opt/local # DarwinPorts
			/opt/csw # Blastwave
			/opt
			/usr/freeware
			/usr/
			/usr/local			
			${SOUND_MODULES_DIR}
	)
ENDFUNCTION(SOUND_FIND_LIBRARY module filename)

SOUND_FIND_LIBRARY(OSGAUDIO_INCLUDE_DIR osgAudio/SoundManager.h)
SOUND_FIND_LIBRARY(OPENALPP_INCLUDE_DIR openalpp/AudioBase.h)
SOUND_FIND_LIBRARY(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)
SOUND_FIND_LIBRARY(OGG_INCLUDE_DIR ogg/ogg.h)
SOUND_FIND_LIBRARY(OPENAL_INCLUDE_DIR al.h)

FUNCTION(SOUND_FIND_LIBRARY module library)
	FIND_LIBRARY(${module} ${library}
		HINTS
			$ENV{SOUND_MODULES_DIR}			
		PATH_SUFFIXES
			lib
			lib64
		PATHS
			/sw # Fink
			/opt/local # DarwinPorts
			/opt/csw # Blastwave
			/opt
			/usr/freeware
			/usr/
			/usr/local			
			${SOUND_MODULES_DIR}
	)
ENDFUNCTION(SOUND_FIND_LIBRARY module library)

SOUND_FIND_LIBRARY(OSGAUDIO_LIBRARY osgAudio)
SOUND_FIND_LIBRARY(OSGAUDIO_LIBRARY_DEBUG osgAudiod)

SOUND_FIND_LIBRARY(OPENALPP_LIBRARY openalpp)
SOUND_FIND_LIBRARY(OPENALPP_LIBRARY_DEBUG openalppd)

SET(SOUND_MODULES_FOUND "NO")
IF(OSGAUDIO_LIBRARY AND OSGAUDIO_INCLUDE_DIR)
  SET(SOUND_MODULES_FOUND "YES")
  SET(OSGAUDIO_LIBRARIES ${OSGAUDIO_LIBRARY} ${OPENALPP_LIBRARY})
ENDIF(OSGAUDIO_LIBRARY AND OSGAUDIO_INCLUDE_DIR)
