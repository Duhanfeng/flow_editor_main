# ----------------------------------------------------------------------------
#   Find Dependencies
# ----------------------------------------------------------------------------

if (WIN32)
    return()
endif ()

message("--------------------------------------------")
message("-")
message("-")
message("-")
message("-")
message("start to find linux dependencies....")

message("find boost...")
include(cmake/linux/find_linux_boost.cmake)
message("find opencv...")
include(cmake/linux/find_linux_opencv.cmake)
message("find fmt...")
include(cmake/linux/find_linux_fmt.cmake)
message("find poco...")
include(cmake/linux/find_linux_poco.cmake)
message("find nlohmann...")
include(cmake/linux/find_linux_nlohmann.cmake)
message("find spdlog...")
include(cmake/linux/find_linux_spdlog.cmake)
message("find qt...")
include(cmake/linux/find_linux_qt.cmake)
message("find vtk...")
include(cmake/linux/find_linux_vtk.cmake)
message("find pcl...")
include(cmake/linux/find_linux_pcl.cmake)
message("find boost...") #导入pcl库后, 会修改boost原有的加载项, 所以最后再加载boost, 避免加载项被覆盖
include(cmake/linux/find_linux_boost.cmake)

message("find linux dependencies finish")

message("-")
message("-")
message("-")
message("-")
message("--------------------------------------------")
