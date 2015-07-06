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

set(ENABLE_TESTS ON)

# Boost
find_package (Boost COMPONENTS unit_test_framework)
if(Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
    add_definitions (-DBOOST_TEST_DYN_LINK)
else()
    set(ENABLE_TESTS OFF)
    message(STATUS "Unit testing disabled")
endif()


#Tests
if (ENABLE_TESTS)
message(STATUS "Test targets enabled")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -fno-inline")
include_directories (${CMAKE_SOURCE_DIR}/lib)
enable_testing ()
set(TEST_TARGETS "")

function(my_test test_name)
    add_executable (${test_name} EXCLUDE_FROM_ALL ${test_name}.cpp ${LIB_SOURCES})
    target_link_libraries( ${test_name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test (NAME ${test_name} COMMAND ${test_name})
    set(TEST_TARGETS ${TEST_TARGETS} ${test_name} PARENT_SCOPE)
endfunction(my_test)

my_test(test_math)
my_test(test_geo)
my_test(test_mirror)

# Coverage
add_custom_target(test_all
    DEPENDS ${TEST_TARGETS}
    COMMENT "Building all tests"
)

set(TRACEFILE "${CMAKE_BINARY_DIR}/test/coverage.info")
add_custom_target(coverage
    COMMAND cd "${CMAKE_BINARY_DIR}/test"
    COMMAND ctest -V
    COMMAND lcov -c -d "${CMAKE_BINARY_DIR}/test" -b "${CMAKE_SOURCE_DIR}" -o ${TRACEFILE} --no-external
    COMMAND genhtml ${TRACEFILE} -o "${CMAKE_BINARY_DIR}/lconv" -p "${CMAKE_SOURCE_DIR}" --demangle-cpp
    DEPENDS ${TEST_TARGETS}
)

add_custom_target(coverage_view
    COMMAND xdg-open "${CMAKE_BINARY_DIR}/lconv/index.html"
)

endif()
