#最低要求的CMake版本为3.10
cmake_minimum_required (VERSION 3.10) 
# 设置项目名
project(OnlineNote LANGUAGES C CXX)


#如果没有指定编译类型，自动设置为Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# C++17
set(CMAKE_CXX_STANDARD 17)
# 设置编译器是否一定要支持该标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置是否启用GCC的拓展功能
set(CMAKE_CXX_EXTENSIONS OFF)
# 设置编译选项
# set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

# 输出目录
set(OUT_DIR "${CMAKE_SOURCE_DIR}/out")

# 头文件目录
set(NOTE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/include")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(NET_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/buduo/include")
set(SQL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/sql")

#目标源码目录
set(NOTE_SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(NET_SRC_DIR "${CMAKE_SOURCE_DIR}/buduo")
set(SQL_SRC_DIR "${CMAKE_SOURCE_DIR}/sql")


#编译目标名
set(SERVER_TARGET server)


#编译子目录
add_subdirectory(${NOTE_SRC_DIR}) 
add_subdirectory(${NET_SRC_DIR}) 
add_subdirectory(${SQL_SRC_DIR})

# 添加头文件查找路径
INCLUDE_DIRECTORIES(${NET_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIR})
INCLUDE_DIRECTORIES(${NOTE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SQL_INCLUDE_DIR})

set(NET_LIB netlib)
set(NOTE_LIB notelib)
set(SQL_LIB sqllib)
# 设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${OUT_DIR})


#设置编译目标
add_executable(${SERVER_TARGET} ./test/server.cpp)

# 添加链接库
# PS:注意链接库的顺序，越基础的库要放在越后面
target_link_libraries(${SERVER_TARGET} ${NOTE_LIB} ${NET_LIB} ${SQL_LIB})



