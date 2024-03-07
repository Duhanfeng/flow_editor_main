# 添加pcl
find_package(PCL REQUIRED)
if (PCL_FOUND)
    message(STATUS "The PCL lib is found! ")
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})

    message(STATUS PCL_INCLUDE_DIRS: ${PCL_INCLUDE_DIRS})
    message(STATUS PCL_LIBRARY_DIRS: ${PCL_LIBRARY_DIRS})
    message(STATUS PCL_DEFINITIONS: ${PCL_DEFINITIONS})
    message(STATUS PCL_VERSION: ${PCL_VERSION})

    message(STATUS "The PCL lib is found! ")
else ()
    message(STATUS "The PCL lib is no found! ")
endif ()