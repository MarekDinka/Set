cmake_minimum_required(VERSION 3.6.2)
project(Set)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(SOURCE_FILES

        TestsSet.cpp
        TestsUniqueSet.cpp
		TestsOrderedSet.cpp
		TestsCombinedSet.cpp
		TestsUniqueCombinedSet.cpp

		GoogleTest/gtest-death-test.cc
        GoogleTest/gtest-filepath.cc
        GoogleTest/gtest-port.cc
        GoogleTest/gtest-printers.cc
        GoogleTest/gtest-test-part.cc
        GoogleTest/gtest-typed-test.cc
        GoogleTest/gtest.cc
        GoogleTest/gtest_main.cc)

include_directories(GoogleTest)
add_executable(set ${SOURCE_FILES})
target_link_libraries(set ${CMAKE_THREAD_LIBS_INIT})