cmake_minimum_required(VERSION 3.10)

project(tp4 C)
set(CMAKE_C_STANDARD 99)
set(HEADER_DIR include/)
file(GLOB HEADER CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/${HEADER_DIR}/*.h)
file(GLOB SOURCE CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.c )
set(CMAKE_BUILD_TYPE Debug)

add_executable(tp4 ${SOURCE} ${HEADER})
set_property(TARGET tp4 PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
target_include_directories(tp4 PRIVATE ${HEADER_DIR})
target_link_libraries(tp4 PRIVATE m)