
# 添加Aruco
find_package(aruco REQUIRED)
if (aruco_FOUND)
    message(STATUS "The aruco lib is found!")
    message(STATUS aruco_LIBS: ${aruco_LIBS})
else ()
    message(STATUS "The aruco lib is no found!")
endif ()
