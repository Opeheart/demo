
set(SRC_PATH ${PROJECT_SOURCE_DIR}/src)

include_directories(${SRC_PATH}/.*h)
add_executable(${PROJECT_NAME} ${SRC_PATH}/*.cpp)

