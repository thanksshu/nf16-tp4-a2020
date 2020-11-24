cmake_minimum_required(VERSION 3.10)

project(TP4 C)
set(CMAKE_C_STANDARD 99)
file(GLOB HEADER include/*.h)
file(GLOB SOURCE src/*.c)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

add_executable(TP4 ${SOURCE})

target_include_directories(TP4 PUBLIC ${HEADER})
target_link_libraries(TP4 PUBLIC m)