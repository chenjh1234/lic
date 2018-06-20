cmake_minimum_required (VERSION 2.8)

project (tetris)

set(JSF_ROOT /home/lxd/mytetris/myjsf)
set(JSF_LD_LIBRARIES jsfserver)
include_directories (${JSF_ROOT}/src/include)
link_directories(${JSF_ROOT}/libso)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -Wno-deprecated")
set(PROJECT_INCLUDE_DIR  ${PROJECT_SOURCE_DIR}/include)

include_directories (${PROJECT_SOURCE_DIR}/../devel)
include_directories (${PROJECT_SOURCE_DIR}/../lib)
include_directories (${PROJECT_SOURCE_DIR}/../libcall)
include_directories (${PROJECT_SOURCE_DIR}/../service/register)
include_directories (${PROJECT_SOURCE_DIR}/../service/um)
include_directories (${PROJECT_SOURCE_DIR}/../service/rm)
include_directories (${PROJECT_SOURCE_DIR}/../service/sm)
include_directories (${PROJECT_SOURCE_DIR}/../service/job/master)
include_directories (${PROJECT_SOURCE_DIR}/../service/devellib)


add_executable (tetrisd tetrisd.cpp)
target_link_libraries (tetrisd tetris)

target_link_libraries (tetrisd tetris-service-register)
target_link_libraries (tetrisd tetris-service-um)
target_link_libraries (tetrisd tetris-service-sm)
target_link_libraries (tetrisd tetris-resource-manager)
target_link_libraries (tetrisd tetris-service-jm-master)
target_link_libraries (tetrisd ${JSF_LD_LIBRARIES})
#target_link_libraries (tetrisd tetris-service-devellib)


add_executable (tetrisc tetrisc.cpp)
target_link_libraries (tetrisc tetris)

target_link_libraries (tetrisc tetris-service-register)
target_link_libraries (tetrisc tetris-service-um)
target_link_libraries (tetrisc tetris-service-sm)
target_link_libraries (tetrisc tetris-resource-manager)
target_link_libraries (tetrisc tetris-service-jm-master)
target_link_libraries (tetrisc ${JSF_LD_LIBRARIES})
target_link_libraries (tetrisc tetris-service-devellib)

add_executable (portal portal.cpp)
target_link_libraries (portal tetris)


add_executable (tetrisu tetrisu.cpp)
target_link_libraries (tetrisu tetris)
target_link_libraries (tetrisu tetris-service-um)


add_executable (simApp simApp.cpp)
target_link_libraries (simApp httpcall)

