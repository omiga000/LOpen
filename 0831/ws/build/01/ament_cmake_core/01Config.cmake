# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_01_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED 01_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(01_FOUND FALSE)
  elseif(NOT 01_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(01_FOUND FALSE)
  endif()
  return()
endif()
set(_01_CONFIG_INCLUDED TRUE)

# output package information
if(NOT 01_FIND_QUIETLY)
  message(STATUS "Found 01: 0.0.0 (${01_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package '01' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${01_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(01_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${01_DIR}/${_extra}")
endforeach()
