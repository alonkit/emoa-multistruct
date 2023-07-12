cmake_minimum_required(VERSION 2.8.3)
project(emoa)

# find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

set(JAVA_INCLUDE_PATH "C:/Program Files/Java/jdk-20")

file(GLOB_RECURSE ALL_HDRS "include/*.hpp")
file(GLOB_RECURSE ALL_SRCS "source/*.cpp")

include_directories(DIRECTORY 
  include/${PROJECT_NAME}
  include
  
)
include_directories(
${JAVA_INCLUDE_PATH}/include
${JAVA_INCLUDE_PATH}/include/win32

)

add_library(${PROJECT_NAME} STATIC
  ${ALL_SRCS}
)

set(test_cpp_dir "test/")
set(test_cpp_files "")
file(GLOB_RECURSE test_cpp_files "${test_cpp_dir}/*.cpp")
foreach(test_cpp_file ${test_cpp_files})
  get_filename_component(test_cpp_name ${test_cpp_file} NAME_WE)
  #ADD_EXECUTABLE(${test_cpp_name} ${ALL_SRCS} ${test_cpp_dir}/${test_cpp_name}.cpp)
  ADD_EXECUTABLE(${test_cpp_name} ${test_cpp_dir}/${test_cpp_name}.cpp)
  TARGET_LINK_LIBRARIES(${test_cpp_name} 
    ${PROJECT_NAME}
    ${JAVA_INCLUDE_PATH}/lib/jvm.lib
  )
endforeach(test_cpp_file ${test_cpp_files})
