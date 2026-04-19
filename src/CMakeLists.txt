
set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)

file(GLOB SRC_FILES "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_PATH})

# 链接 Qt 库
target_link_libraries(${CMAKE_PROJECT_NAME}
PRIVATE
    ${QT_LIB_PREFIX}Core
    ${QT_LIB_PREFIX}Widgets
    ${QT_LIB_PREFIX}Gui
)


# target_link_libraries(${CMAKE_PROJECT_NAME}
# PUBLIC
#     ${QT_LIB_PREFIX}Core
#     ${QT_LIB_PREFIX}Widgets
#     ${QT_LIB_PREFIX}Gui
#     if(WIN32)
#     PRIVATE Ws2_32
# endif()
# )

# if(WIN32)
#     target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Ws2_32)
# endif()

# 包含拷贝 Qt DLL 的逻辑
include(${CMAKE_SOURCE_DIR}/cmake/copy_qt_dlls.cmake)
