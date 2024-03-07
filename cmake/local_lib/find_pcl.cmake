
# 添加boost
if (WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${3rdparty_dir}/pcl-1.14.0/cmake")

    # 添加FLANN
    set(FLANN_INCLUDE_DIR ${3rdparty_dir}/FLANN/include)
    set(FLANN_LIBRARY_STATIC ${3rdparty_dir}/FLANN/lib/flann_cpp_s.lib)
    include_directories(${FLANN_INCLUDE_DIR})
    set(PCL_FLANN_REQUIRED_TYPE STATIC)

    # 添加QHULL
    set(QHULL_INCLUDE_DIR ${3rdparty_dir}/Qhull/include)
    set(QHULL_LIBRARY ${3rdparty_dir}/Qhull/lib/qhullstatic.lib)
    set(PCL_QHULL_REQUIRED_TYPE STATIC)
    set(QHULL_LIBRARY_TYPE STATIC)
endif ()

find_package(PCL 1.14 REQUIRED)
if (PCL_FOUND)
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})

    message(STATUS PCL_ROOT: ${PCL_ROOT})
    message(STATUS PCL_INCLUDE_DIRS: ${PCL_INCLUDE_DIRS})
    message(STATUS PCL_LIBRARY_DIRS: ${PCL_LIBRARY_DIRS})
    message(STATUS PCL_DEFINITIONS: ${PCL_DEFINITIONS})
    message(STATUS PCL_LIBRARIES: ${PCL_LIBRARIES})
    message(STATUS PCL_VERSION: ${PCL_VERSION})

    message(STATUS "The PCL lib is found! ")
else ()
    message(STATUS "The PCL lib is no found! ")
endif ()
