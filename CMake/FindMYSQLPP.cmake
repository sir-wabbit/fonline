# - Try to find mysqlpp
# Once done this will define
#
#  MYSQLPP_FOUND - system has mysqlpp
#  MYSQLPP_INCLUDE_DIR - the mysqlpp include directory

# Be quiet if mysqlpp was already found
IF (mysqlpp_INCLUDE_DIR AND mysqlpp_LIBRARY)
    SET(mysqlpp_QUIET TRUE)
ENDIF (mysqlpp_INCLUDE_DIR AND mysqlpp_LIBRARY)


FIND_PATH(mysqlpp_INCLUDE_DIR
    NAMES mysql++.h
    PATH_SUFFIXES mysql++
    )

FIND_LIBRARY(mysqlpp_LIBRARY
    NAMES mysqlpp
    PATHS /usr/lib /usr/local/lib
    )

MARK_AS_ADVANCED(mysqlpp_INCLUDE_DIR mysqlpp_LIBRARY)


IF (mysqlpp_INCLUDE_DIR AND mysqlpp_LIBRARY)
    SET(MYSQLPP_FOUND TRUE)
    SET(MYSQLPP_INCLUDE_DIR ${mysqlpp_INCLUDE_DIR})
    SET(MYSQLPP_LIBRARIES ${mysqlpp_LIBRARY})

    IF (NOT mysqlpp_FIND_QUIETLY AND NOT mysqlpp_QUIET)
        MESSAGE(STATUS "Found mysqlpp: ${mysqlpp_LIBRARY}")
    ENDIF (NOT mysqlpp_FIND_QUIETLY AND NOT mysqlpp_QUIET)
ENDIF (mysqlpp_INCLUDE_DIR AND mysqlpp_LIBRARY)

# Bail out if mysqlpp is not found but required
IF (NOT MYSQLPP_FOUND AND mysqlpp_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could NOT find mysqlpp library ${MYSQLPP_INCLUDE_DIR}")
ENDIF (NOT MYSQLPP_FOUND AND mysqlpp_FIND_REQUIRED)
