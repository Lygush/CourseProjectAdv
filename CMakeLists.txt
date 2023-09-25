cmake_minimum_required(VERSION 3.2)
project(CourseProjectAdv)

set(SOURCES CourseProjectAdv.cpp IniParser.cpp IniError.cpp)
set(HEADERS IniParser.h IniError.h)

add_executable(CourseProjectAdv ${SOURCES} ${HEADERS})