# ------------------------------------------------------------------------------
# Qt DLL 自动拷贝功能
# ------------------------------------------------------------------------------
function(copy_qt_dlls target_name)
    # 只有在 Windows 上才需要拷贝 DLL
    if(WIN32)
        # 获取目标可执行文件的输出目录
        get_target_property(TARGET_OUTPUT_DIR ${target_name} RUNTIME_OUTPUT_DIRECTORY)
        if(NOT TARGET_OUTPUT_DIR)
            set(TARGET_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        endif()

        # 确保输出目录存在
        file(MAKE_DIRECTORY ${TARGET_OUTPUT_DIR})

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

            # 添加 post-build 命令来拷贝 Qt DLLs
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE}
                        --no-translations
                        --no-system-d3d-compiler
                        --no-compiler-runtime
                        --no-opengl-sw
                        $<TARGET_FILE:${target_name}>
                WORKING_DIRECTORY ${TARGET_OUTPUT_DIR}
                COMMENT "Copying Qt DLLs to executable directory using windeployqt"
            )
        else()
            message(WARNING "windeployqt not found at: ${WINDEPLOYQT_EXECUTABLE}")
            message(WARNING "Qt DLLs will not be automatically copied.")
            message(WARNING "You may need to manually copy Qt DLLs or set QT_PATH correctly.")
        endif()
    endif()
endfunction()