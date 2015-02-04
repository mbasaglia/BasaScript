# Copyright (C) 2015  Mattia Basaglia
#
# This is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
project(BasaScript)
cmake_minimum_required(VERSION 2.8)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (MAINTAINER "Mattia Basaglia <mattia.basaglia@gmail.com>")

# Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic")
include_directories("${CMAKE_SOURCE_DIR}/src")

# Enable Debug by default, can be changed with -D CMAKE_BUILD_TYPE=Release
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    set(DEBUG 1)
else()
    set(DEBUG 0)
endif()

# Check C++11
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(--std=c++11 STD_CXX11)
    if(STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
        set(HAS_STD_CXX11 1)
    else()
        message(SEND_ERROR "C++11 or better is required")
    endif()
endif()

# Check Libraries
find_package(Boost 1.54 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  include(CheckIncludeFileCXX)
  CHECK_INCLUDE_FILE_CXX("boost/multiprecision/cpp_dec_float.hpp" BOOST_cpp_dec_float)
  CHECK_INCLUDE_FILE_CXX("boost/math/constants/constants.hpp" BOOST_constants)
  if (NOT (BOOST_cpp_dec_float AND BOOST_constants))
    message(SEND_ERROR "Missing required boost libraries")
    unset(BOOST_cpp_dec_float CACHE)
  endif()
endif()

# Sources
file(GLOB_RECURSE LIB_SOURCES lib/*.cpp)
file(GLOB_RECURSE LIB_HEADERS lib/*.hpp)

file(GLOB_RECURSE BIN_SOURCES src/*.cpp)
file(GLOB_RECURSE BIN_HEADERS src/*.hpp)

set(ALL_SOURCES ${LIB_SOURCES} ${LIB_HEADERS} ${BIN_SOURCES} ${BIN_HEADERS})
add_executable(${PROJECT_NAME} ${LIB_SOURCES} ${BIN_SOURCES})

# Subdirs
add_subdirectory(test) # Unit tests

# Extra stuff

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT doc)
    foreach(source ${ALL_SOURCES})
	set(DOXYGEN_INPUT "${DOXYGEN_INPUT} ${source}")
    endforeach()
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)
    add_custom_target(doc
	    ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
	    DEPENDS ${PROJECT_BINARY_DIR}/Doxyfile ${ALL_SOURCES}
	    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	    COMMENT "Generating Doxygen Documentation" VERBATIM
    )
    add_custom_target(doc-view
	    xdg-open ${DOXYGEN_OUTPUT}/html/index.html
	    COMMENT "Showing Doxygen documentation"
    )
endif(DOXYGEN_FOUND)
