# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 项目信息
project(CodeCraft-2021)

include_directories(${PROJECT_SOURCE_DIR}/lib/)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
file(GLOB DIR_SRCS ./lib/*)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

# 指定生成目标
add_executable(CodeCraft-2021 CodeCraft-2021.cpp)
add_executable(test test.cpp)
add_executable(judge toolkit/judge.cpp toolkit/OfflineJudger.hpp)