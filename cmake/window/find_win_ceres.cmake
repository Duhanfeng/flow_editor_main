
# 添加Ceres
find_package(Ceres REQUIRED)
if (EXISTS ${CUSTOM_LIB_DIR}/Ceres)
    include_directories("${CUSTOM_LIB_DIR}/Ceres/include")
    link_directories("${CUSTOM_LIB_DIR}/Ceres/lib")
    message(STATUS "The Ceres lib is found!")
else ()
    message(STATUS "The Ceres lib is no found!")
endif ()
