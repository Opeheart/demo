# copy_qt_dlls.cmake
# 创建独立的拷贝 Qt DLL 目标（仅在 Windows 上）

if(WIN32)
    # 获取目标可执行文件的输出目录
    get_target_property(TARGET_OUTPUT_DIR ${CMAKE_PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
    if(NOT TARGET_OUTPUT_DIR)
        set(TARGET_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()

    # 根据 Qt 版本获取 windeployqt 工具
    if(QT_VERSION_MAJOR STREQUAL "6")
        get_target_property(QT_QMAKE_LOCATION Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(QT_BIN_DIR "${QT_QMAKE_LOCATION}" DIRECTORY)
        set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")
    elseif(QT_VERSION_MAJOR STREQUAL "5")
        get_target_property(QT_QMAKE_LOCATION Qt5::qmake IMPORTED_LOCATION)
        get_filename_component(QT_BIN_DIR "${QT_QMAKE_LOCATION}" DIRECTORY)
        set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")
    endif()

    # 检查 windeployqt 是否存在
    if(EXISTS "${WINDEPLOYQT_EXECUTABLE}")
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")

        # 创建独立的自定义目标来拷贝 Qt DLLs
        add_custom_target(copy_qt_dlls
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                    --no-translations
                    --no-system-d3d-compiler
                    --no-compiler-runtime
                    --no-opengl-sw
                    $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            WORKING_DIRECTORY ${TARGET_OUTPUT_DIR}
            COMMENT "Copying Qt DLLs to executable directory using windeployqt"
            DEPENDS ${CMAKE_PROJECT_NAME}
        )
    else()
        message(WARNING "windeployqt not found. Qt DLLs will not be copied automatically.")
    endif()
endif()