
# 添加Eigen
find_package(Eigen3 REQUIRED)
if (Eigen3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})

    # 兼容性调整
    set(EIGEN_FOUND             ${EIGEN3_FOUND})
    set(EIGEN_USE_FILE          ${EIGEN3_USE_FILE})
    set(EIGEN_DEFINITIONS       ${EIGEN3_DEFINITIONS})
    set(EIGEN_INCLUDE_DIR       ${EIGEN3_INCLUDE_DIR})
    set(EIGEN_INCLUDE_DIRS      ${EIGEN3_INCLUDE_DIRS})
    set(EIGEN_ROOT_DIR          ${EIGEN3_ROOT_DIR})
    set(EIGEN_VERSION_STRING    ${EIGEN3_VERSION_STRING})
    set(EIGEN_VERSION_MAJOR     ${EIGEN3_VERSION_MAJOR})
    set(EIGEN_VERSION_MINOR     ${EIGEN3_VERSION_MINOR})
    set(EIGEN_VERSION_PATCH     ${EIGEN3_VERSION_PATCH})

    message(STATUS EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR})
    message(STATUS EIGEN3_DEFINITIONS: ${EIGEN3_DEFINITIONS})
    message(STATUS EIGEN3_VERSION_STRING: ${EIGEN3_VERSION_STRING})
    message(STATUS "The Eigen3 lib is found!")
else ()
    message(STATUS "The Eigen3 lib is no found!")
endif ()