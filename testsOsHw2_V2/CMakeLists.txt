cmake_minimum_required(VERSION 3.0.0)
project(os_hw2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(os_hw2 test1.cxx hw2_test.cxx)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
