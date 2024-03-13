cmake_minimum_required(VERSION 3.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

#################################################################################
# 设置C++编译器选项
#################################################################################

#################################################################################
# 第三方库
#################################################################################

include(${CMAKE_SOURCE_DIR}/cmake/window/find_win_qt.cmake)

#################################################################################
# 系统参数
#################################################################################

#################################################################################
# 添加工程路径
#################################################################################

set(PROJECT_SOURCES
        main.cxx
        flow.hpp
        nodes_creator.hpp
        util/guid_creator.hpp
        util/random_string.hpp
        util/style_creator.hpp
)

# 添加rcc
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message(qt6)
    qt_add_resources(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rcc/icon.qrc)
else ()
    message(qt5)
    qt5_add_resources(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rcc/icon.qrc)
endif ()
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}
        flow_editor
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

####################################################################################################
# 安装
####################################################################################################

####################################################################################################
# 设置生成时的操作
####################################################################################################

if (EXISTS ${CMAKE_SOURCE_DIR}/data/install/review_glqt/bin/review_glqt.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            #拷贝动态链接库到打包路径下
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/data/install/review_glqt/bin/review_glqt.dll" "${PROJECT_BINARY_DIR}/review_glqt.dll"
    )
endif ()
