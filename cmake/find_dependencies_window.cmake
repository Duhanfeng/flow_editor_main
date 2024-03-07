# ----------------------------------------------------------------------------
#   Find Dependencies
# ----------------------------------------------------------------------------

if (NOT WIN32)
    return()
endif ()

message("--------------------------------------------")
message("-")
message("-")
message("-")
message("-")
message("start to find window dependencies....")

set(3rdparty_dir ${CMAKE_SOURCE_DIR}/3rdparty)

set(CUSTOM_LIB_DIR "C:/Program Files/CppLibrary")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${3rdparty_dir}/opencv")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${3rdparty_dir}/eigen/share/eigen3/cmake")

message("find openmp...")
include(cmake/window/find_win_openmp.cmake)
#message("find cgal...")
#include(cmake/window/find_win_cgal.cmake)
#message("find eigen...")
#include(cmake/local_lib/find_eigen.cmake)
#message("find boost...")
#include(cmake/local_lib/find_boost.cmake)
#message("find opencv...")
#include(cmake/local_lib/find_opencv.cmake)
#message("find fmt...")
#include(cmake/local_lib/find_fmt.cmake)
#message("find nlohmann...")
#include(cmake/local_lib/find_nlohmann.cmake)
#message("find spdlog...")
#include(cmake/local_lib/find_spdlog.cmake)
#message("find pcl...")
#include(cmake/local_lib/find_pcl.cmake)
#message("find boost...") #导入pcl库后, 会修改boost原有的加载项, 所以最后再加载boost, 避免加载项被覆盖
#include(cmake/local_lib/find_boost.cmake)
#message("find local minizip-ng...")
#include(cmake/local_lib/find_minizip_ng.cmake)
#message("find local review_glqt...")
#include(cmake/local_lib/find_review_glqt.cmake)

message("find qt...")
include(cmake/window/find_win_qt.cmake)

message("find window dependencies finish")

message("-")
message("-")
message("-")
message("-")
message("--------------------------------------------")
