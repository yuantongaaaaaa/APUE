cmake_minimum_required(VERSION 3.24)          # 指定CMake最低版本[reference:23]
project(APUE)                         # 定义项目名称[reference:24]
set(CMAKE_CXX_STANDARD 17)                    # 设置C++标准，例如C++17[reference:25]


# 将 src/ 目录下的所有 .c 文件都添加进来
# 分别收集每个模块的源文件
file(GLOB MAIN_SOURCES "*.cpp" "*.c")
file(GLOB DRIVER_SOURCES "driver/*.cpp" "driver/*.c")
file(GLOB IO_SOURCES "io/*.cpp" "io/*.c")
file(GLOB MEMORY_SOURCES "memorypool/*.cpp" "memorypool/*.c")
file(GLOB NET_SOURCES "net/*.cpp" "net/*.c")
file(GLOB THREAD_SOURCES "threadpool/*.cpp" "threadpool/*.c")


#2.可执行文件,合并所有源文件
add_executable(APUE
        ${MAIN_SOURCES}
        ${DRIVER_SOURCES}
        ${IO_SOURCES}
        ${MEMORY_SOURCES}
        ${NET_SOURCES}
        ${THREAD_SOURCES}
)

# 3. 添加头文件搜索路径（关键！让 #include "xxx.h" 能找到头文件，没有必要直接写绝对路径）
# 方法A：把根目录和所有子目录都加进去（推荐）
target_include_directories(APUE PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}          # 根目录
        ${CMAKE_CURRENT_SOURCE_DIR}/driver
        ${CMAKE_CURRENT_SOURCE_DIR}/io
        ${CMAKE_CURRENT_SOURCE_DIR}/memorypool
        ${CMAKE_CURRENT_SOURCE_DIR}/net
        ${CMAKE_CURRENT_SOURCE_DIR}/threadpool
)