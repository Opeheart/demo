# ------------------------------------------------------------------------------
# 用户可配置选项（在 CMake GUI 中显示）
# ------------------------------------------------------------------------------

# 1. Qt 安装路径（必须指向包含 lib/cmake/Qt6 或 lib/cmake/Qt5 的目录）
set(QT_PATH "" CACHE PATH "Path to Qt installation (e.g., D:/Qt/6.8.0/msvc2022_64)")

# 2. 选择 Qt 主版本（5 或 6）
set(QT_VERSION_MAJOR "6" CACHE STRING "Qt major version (5 or 6)")
set_property(CACHE QT_VERSION_MAJOR PROPERTY STRINGS 5 6)

# 配置 Qt 的 CMake 模块路径
if(QT_PATH)
    # 根据版本构造 CMake 包的路径
    if(QT_VERSION_MAJOR STREQUAL "6")
        set(Qt6_DIR "${QT_PATH}/lib/cmake/Qt6" CACHE PATH "Path to Qt6Config.cmake")
    elseif(QT_VERSION_MAJOR STREQUAL "5")
        set(Qt5_DIR "${QT_PATH}/lib/cmake/Qt5" CACHE PATH "Path to Qt5Config.cmake")
    endif()
    # 同时设置 CMAKE_PREFIX_PATH 以确保 find_package 能找到依赖
    list(APPEND CMAKE_PREFIX_PATH "${QT_PATH}")
else()
    message(WARNING "QT_PATH is not set. CMake will search in system PATH.")
    message(WARNING "Please set QT_PATH in CMake GUI or: -DQT_PATH=/path/to/Qt/installation")
endif()

# 调试信息
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "Qt6_DIR: ${Qt6_DIR}")
message(STATUS "Qt5_DIR: ${Qt5_DIR}")

# ------------------------------------------------------------------------------
# 查找 Qt 包
# ------------------------------------------------------------------------------
if(QT_VERSION_MAJOR STREQUAL "6")
    # 要求 Qt6 及其组件
    find_package(Qt6 6.0 REQUIRED COMPONENTS Core Widgets Gui)
    # 设置链接目标的前缀
    set(QT_LIB_PREFIX "Qt6::")
elseif(QT_VERSION_MAJOR STREQUAL "5")
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Gui)
    set(QT_LIB_PREFIX "Qt5::")
else()
    message(FATAL_ERROR "Invalid QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}. Must be 5 or 6.")
endif()

# 打印配置信息（可选）
message(STATUS "Qt version: ${QT_VERSION_MAJOR}")
message(STATUS "Qt path: ${QT_PATH}")

# 将 QT_LIB_PREFIX 标记为缓存变量，确保正确的作用域传播
set(QT_LIB_PREFIX "${QT_LIB_PREFIX}" CACHE INTERNAL "Qt Library Prefix")

# 包含 Qt DLL 拷贝功能
include(copyQtDlls)
