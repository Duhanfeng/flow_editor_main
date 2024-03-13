cmake_minimum_required(VERSION 3.8)

project(flow_editor)

#################################################################################
# 设置C++编译器选项
#################################################################################

#################################################################################
# 第三方库
#################################################################################

#################################################################################
# 系统参数
#################################################################################

set(BUILD_RS_SHARED_LIBS OFF)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#################################################################################
# 添加工程路径
#################################################################################

set(PROJECT_INCLUDE
        include/flow_editor/flow_editor.hpp
        include/flow_editor/data_def.hpp
        include/flow_editor/flow.hpp
        include/flow_editor/flow_view/flow_view.hpp
        include/flow_editor/flow_view/flow_scene.hpp
)

set(PROJECT_SOURCES
        ${PROJECT_INCLUDE}
        src/flow_view/flow_view.cxx
        src/flow_view/flow_scene.cxx
        src/flow_view/flow_scene_data.cxx
        src/flow_view/flow_scene_data.hpp
        src/flow_view/default_style.hpp
        src/node_sub_geometry/node_sub_geometry.hpp
        src/node_sub_geometry/dynamic_geometry.hpp
        src/node_sub_geometry/dynamic_geometry.cxx
        src/node_sub_geometry/dynamic_h_port_geometry.hpp
        src/node_sub_geometry/dynamic_h_port_geometry.cxx
        src/items/abs_node_item.hpp
        src/items/node_item.hpp
        src/items/node_item.cxx
        src/items/in_node_item.hpp
        src/items/in_node_item.cxx
        src/items/out_node_item.hpp
        src/items/out_node_item.cxx
        src/items/connection/connection_item.cxx
        src/items/connection/connection_item.hpp
        src/items/connection/draft_connection_item.cxx
        src/items/connection/draft_connection_item.hpp
        src/items/painter/node_painter.hpp
        src/items/painter/node_painter.cxx
        src/items/painter/in_node_painter.hpp
        src/items/painter/in_node_painter.cxx
        src/items/painter/out_node_painter.hpp
        src/items/painter/out_node_painter.cxx
)
# 添加rcc
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message(qt6)
    qt_add_resources(PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/data/rcc/font.qrc)
else ()
    message(qt5)
    qt5_add_resources(PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/data/rcc/font.qrc)
endif ()

# 将源代码添加到此项目的可执行文件。
if (BUILD_RS_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_RESOURCES_STATIC})
else ()
    add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_RESOURCES_STATIC})
endif ()
target_link_libraries(
        ${PROJECT_NAME}
        ${Boost_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Widgets
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)
if (BUILD_RS_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RS_EXPORT_SHARE)
endif ()

####################################################################################################
# 安装
####################################################################################################

# 设置输出根路径
set(RS_INSTALL_PATH ${CMAKE_SOURCE_DIR}/data/install/re_scaffold)

# 设置库安装路径
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${RS_INSTALL_PATH}/lib  # 动态库安装路径
        ARCHIVE DESTINATION ${RS_INSTALL_PATH}/lib  # 静态库安装路径
        RUNTIME DESTINATION ${RS_INSTALL_PATH}/bin  # 可执行文件安装路径
)

# 复制相关头文件到头文件安装路径下
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/re_scaffold DESTINATION ${RS_INSTALL_PATH}/include)
