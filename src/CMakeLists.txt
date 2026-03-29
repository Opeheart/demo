
set(SRC_PATH ${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SRC_PATH}/*.cpp)

target_include_directories(${SRC_PATH})

