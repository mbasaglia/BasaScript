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
cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${CMAKE_SOURCE_DIR}/lib ${Boost_INCLUDE_DIRS})
add_definitions (-DBOOST_TEST_DYN_LINK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -Wextra -pedantic ")
enable_testing ()

add_executable (test_math test_math.cpp ${LIB_SOURCES})
target_link_libraries( test_math ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME test_math COMMAND test_math)
