set(PROJECT_NAME utils)
set(LIB_NAME ${PROJECT_NAME})
set(UNIT_TEST_NAME unit_test_${PROJECT_NAME})
cmake_minimum_required(VERSION 2.6)
project(utils)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tool)
include_directories(${DAC_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../DAC/src)

link_directories(${PROJECT_SOURCE_DIR}/../DAC/build)

file(GLOB SRC src/*.cpp)
add_library(utils ${SRC})

file(GLOB SRC_TEST test/*.cpp)
add_executable(${UNIT_TEST_NAME} ${SRC_TEST})
target_link_libraries(${UNIT_TEST_NAME} pthread gtest gtest_main)
target_link_libraries(${UNIT_TEST_NAME} utils DAC)

add_custom_target(test
  COMMAND ${UNIT_TEST_NAME}
  DEPENDS ${UNIT_TEST_NAME}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

macro(addtool)
  file(GLOB SRC_TOOL tool/*.cpp)
  foreach(tool ${SRC_TOOL})
    file(RELATIVE_PATH variable ${PROJECT_SOURCE_DIR}/tool ${tool})
    get_filename_component(variable ${variable} NAME_WE)
    add_executable(${variable} tool/${variable})
    target_link_libraries(${variable} utils DAC leveldb sdsl marisa pthread snappy)
  endforeach(tool)
endmacro(addtool)
addtool()


