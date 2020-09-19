cmake_minimum_required(VERSION 3.16)
set(EXECUTABLE_EXE_NAME sudoku)
project(${EXECUTABLE_EXE_NAME})

set(CMAKE_CXX_STANDARD 17)

# 可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ../bin)

# main函数
set(SOURCES
    src/main.cpp
)

# 把源文件添加进工程中
add_executable(${EXECUTABLE_EXE_NAME} ${SOURCES}
)

# 设置包含路径
target_include_directories(${EXECUTABLE_EXE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)