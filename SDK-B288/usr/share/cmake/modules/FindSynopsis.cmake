# -*- cmake -*-

# - Find Synopsis library
# Find the Synopsis includes and library
# This module defines
# SYNOPSIS_INCLUDE_DIR, where to find g2log.hpp, etc.
# SYNOPSIS_LIBRARIES, the libraries needed to use libsynopsis.so
# SYNOPSIS_FOUND, If false, do not try to use Synopsis.

FIND_PATH(SYNOPSIS_INCLUDE_DIR synopsis
/include
/usr/include
/usr/local/include
)

FIND_LIBRARY(SYNOPSIS_LIBRARIES
  NAMES libsynopsis.so
  PATHS /lib /usr/lib /usr/local/lib
)

FIND_LIBRARY(SYNOPSIS_RECOMMENDATION_LIBRARIES
  NAMES librecommendation.so
  PATHS /lib /usr/lib /usr/local/lib
)


IF (SYNOPSIS_LIBRARIES AND SYNOPSIS_INCLUDE_DIR)
  SET(SYNOPSIS_FOUND "YES")
ELSE (SYNOPSIS_LIBRARIES AND SYNOPSIS_INCLUDE_DIR)
  SET(SYNOPSIS_FOUND "NO")
ENDIF (SYNOPSIS_LIBRARIES AND SYNOPSIS_INCLUDE_DIR)

IF (SYNOPSIS_FOUND)
  IF (NOT SYNOPSIS_FIND_QUIETLY)
    MESSAGE(STATUS "Found SYNOPSIS: ${SYNOPSIS_LIBRARIES}")
    MESSAGE(STATUS "Found SYNOPSIS_RECOMMENDATION: ${SYNOPSIS_RECOMMENDATION_LIBRARIES}")
  ENDIF (NOT SYNOPSIS_FIND_QUIETLY)
ELSE (SYNOPSIS_FOUND)
  IF (SYNOPSIS_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find SYNOPSIS library")
  ENDIF (SYNOPSIS_FIND_REQUIRED)
ENDIF (SYNOPSIS_FOUND)