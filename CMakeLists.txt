# 最低cmake版本
cmake_minimum_required(VERSION 3.18)

# 设置项目名称和语言
project(SmartButlerForRaspberry CXX)

# 寻找wiringPi库
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)

# 自定义头文件路径
include_directories(include)
# 二进制文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 获取源码的路径
aux_source_directory(src src)

# 编译标准C++20和反馈参数
add_compile_options(-std=c++20 -Wall -Werror -Wextra -pedantic -Wimplicit-fallthrough -Wsequence-point -Wswitch-default -Wswitch-unreachable -Wswitch-enum -Wstringop-truncation -Wbool-compare -Wtautological-compare -Wfloat-equal -Wshadow=global -Wpointer-arith -Wpointer-compare -Wcast-align -Wcast-qual -Wwrite-strings -Wdangling-else -Wlogical-op -Wconversion -Wno-pedantic-ms-format)

# 构建二进制文件
add_executable(.SmartButler ${src})

# 链接wiringPi库到二进制文件
target_link_libraries(.SmartButler ${WIRINGPI_LIBRARIES})