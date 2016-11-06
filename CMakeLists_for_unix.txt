project(DWRenderer)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -ggdb -O0 -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# headers
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/headers)
# external libs and tools
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
# GLM
set(GLM_INC ${EXTERNAL_DIR}/glm)

find_package(OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Qui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

set(Qt5_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES})

FILE(GLOB folder_shader  shaders/*.* shaders/*/*.*)
FILE(GLOB folder_project_include headers/*.* headers/*/*.*)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${GLM_INC})

add_executable(${PROJECT_NAME} ${SRC_LIST} ${folder_shader} ${folder_project_include})

target_link_libraries(${PROJECT_NAME} ${Qt5_LIBRARIES} ${OPENGL_LIBRARIES})
