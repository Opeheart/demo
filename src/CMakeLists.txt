
set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)

file(GLOB SRC_FILES "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_PATH})

# 链接 Qt 库
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${QT_LIB_PREFIX}Core
    ${QT_LIB_PREFIX}Widgets
    ${QT_LIB_PREFIX}Gui
)

# 自动拷贝 Qt DLLs 到可执行文件目录（仅在 Windows 上）
copy_qt_dlls(${CMAKE_PROJECT_NAME})
