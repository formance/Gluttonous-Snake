cmake_minimum_required(VERSION 3.1.6)

project(cpp_test)

#build type
set(CMAKE_BUILD_TYPE "Release")

#OpenCV Libraries
find_package(OpenCV 3 REQUIRED)
# message(STATUS "OpenCV library status:")
# message(STATUS "    version: ${OpenCV_VERSION}")
# message(STATUS "    libraries: ${OpenCV_LIBS}")
# message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(cpp_test demo.cpp)
target_link_libraries(cpp_test ${OpenCV_LIBS} pthread)

# add_executable(kb_test kb_test.cpp)

