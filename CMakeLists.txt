
cmake_minimum_required(VERSION 4.1)

project(bytecodevm C)


file(GLOB SOURCE "*.c")

add_executable(bytecodevm ${SOURCE})

