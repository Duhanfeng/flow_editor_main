
# 添加Ceres
find_package(Ceres REQUIRED)
if (Ceres_FOUND)
    message(STATUS "The Ceres lib is found!")
    message(CERES_INCLUDE_DIRS: ${CERES_INCLUDE_DIRS})
    message(CERES_INCLUDES: ${CERES_INCLUDES})
    message(CERES_LIBRARIES: ${CERES_LIBRARIES})
    include_directories(${CERES_INCLUDE_DIRS})
else ()
    message(STATUS "The Ceres lib is no found!")
endif ()