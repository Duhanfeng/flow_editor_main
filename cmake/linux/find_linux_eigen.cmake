
# 添加Eigen
find_package(Eigen3 REQUIRED)
if (Eigen3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})
    message(STATUS "The Eigen3 lib is found!")
    message(STATUS EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR})
else ()
    message(STATUS "The Eigen3 lib is no found!")
endif ()
