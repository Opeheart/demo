# @Author: Opeheart w7302360@163.com
# @Date: 2026-03-27 21:38:15
# @LastEditors: Opeheart w7302360@163.com
# @LastEditTime: 2026-03-27 21:50:33
# @FilePath: \1_testGit\demo\CMakeLists.txt
# @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
cmake_minimum_required(VERSION 3.10)

project(SArchitect LANGUAGES C CXX)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_DESCRIPTION "A demo for CMake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(configQtEnvironment)

# 在 Qt 配置加载后设置 Qt 相关的自动处理选项
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
include(applicationPrepare)
