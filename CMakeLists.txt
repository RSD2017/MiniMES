#####################################################
# Template for building RobWork dependent projects
# - the src should contain code for putting into libraries 
# - the plugins contain code for multiple RobWorkStudio plugins
# - the test for testing 
# - the example dir for illustrativ examples of your project
#
# Use config.cmake to change default settings and edit this
# file for adding additional dependencies to external projects  
#####################################################

#
# Test CMake version
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

# The name of the project.
PROJECT(MiniMES)

# Used to resolve absolute path names
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_VERBOSE_MAKEFILE True)

########################################################################
#### Make use of c++11
########################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(BOOST_ROOT "D:/boost_1_58_0")
  SET(Boost_USE_STATIC_LIBS ON)
  SET(BOOST_ALL_DYN_LINK OFF)
  FIND_PACKAGE(Boost COMPONENTS filesystem regex serialization system thread program_options chrono)



#find_package(Boost COMPONENTS thread)
#if(Boost_FOUND)
#  MESSAGE("Fouind Boost ")
#  include_directories(${Boost_INCLUDE_DIRS})
#endif()

# Now set the RW/RWS root (edit this if necessary)
add_subdirectory(src)



