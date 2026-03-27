# @Author: Opeheart w7302360@163.com
# @Date: 2026-03-27 21:38:15
# @LastEditors: Opeheart w7302360@163.com
# @LastEditTime: 2026-03-27 21:50:33
# @FilePath: \1_testGit\demo\CMakeLists.txt
# @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
cmake_minimum_required(3.2)

project(demo)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_DESCRIPTION "A demo for CMake")

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)
