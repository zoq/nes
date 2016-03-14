# Module for finding the core components of OpenCV installed by
# graphics/opencv-core.  Use for projects that require only
# opencv_core or opencv_imgproc.  Others should use the
# standard OpenCV CMake find routines provided by graphics/opencv.
#
# This module provides:
# OPENCVCORE_FOUND - defined if the required OpenCV components are found
# OpenCV_INCLUDE_DIRS - the path to the OpenCV headers
# OpenCV_LIBS - the OpenCV libraries to link to
# OpenCV_VERSION - the version of OpenCV
#
# Example usage: find_package( OpenCVCore COMPONENTS core imgproc REQUIRED )

include( FindPackageHandleStandardArgs )

find_file(OPENCV_CONFIG_CMAKE NAMES OpenCVConfig.cmake
  PATHS
  ${OPENCV_CMAKE_PATHS}
)

if(EXISTS ${OPENCV_CONFIG_CMAKE})
    include(${OPENCV_CONFIG_CMAKE})
endif()

find_file(OPENCV_CONFIG_VERSION_CMAKE NAMES OpenCVConfig-version.cmake
  PATHS
  ${OPENCV_CMAKE_PATHS}
)

if(EXISTS ${OPENCV_CONFIG_VERSION_CMAKE})
    include(${OPENCV_CONFIG_VERSION_CMAKE})
endif()

find_path( OpenCV_INCLUDE_DIRS NAMES opencv2/core/core.hpp )

if( OpenCVCore_FIND_COMPONENTS )
  foreach( component ${OpenCVCore_FIND_COMPONENTS} )
    string( TOUPPER ${component} _COMPONENT )
    set( OPENCV_USE_${_COMPONENT} 1 )
  endforeach()
endif()

# opencv_core is always required
find_library( OPENCV_CORE_LIBRARY NAMES opencv_core )

if( OPENCV_USE_IMGPROC OR NOT OpenCVCore_FIND_COMPONENTS )
  find_library( OPENCV_IMGPROC_LIBRARY NAMES opencv_imgproc )
  find_library( OPENCV_HIGHGUI_LIBRARY NAMES opencv_highgui )

  if(OpenCV_VERSION VERSION_LESS "3.0.0")
    set( OpenCV_LIBS ${OPENCV_CORE_LIBRARY} ${OPENCV_IMGPROC_LIBRARY} ${OPENCV_HIGHGUI_LIBRARY} )
  else()
    find_library( OPENCV_IMGCODECS_LIBRARY NAMES opencv_imgcodecs )
    set( OpenCV_LIBS ${OPENCV_CORE_LIBRARY} ${OPENCV_IMGPROC_LIBRARY} ${OPENCV_HIGHGUI_LIBRARY} ${OPENCV_IMGCODECS_LIBRARY} )
  endif()
endif()

find_package_handle_standard_args( OpenCVCore DEFAULT_MSG OpenCV_LIBS OpenCV_INCLUDE_DIRS )

mark_as_advanced( ${OpenCV_LIBS} ${OpenCV_INCLUDE_DIRS} ${OpenCV_VERSION} )