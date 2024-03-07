
# 添加nlohmann
if (WIN32)
    if (EXISTS ${CUSTOM_LIB_DIR}/CGAL-5.5.2)
        include_directories(${CUSTOM_LIB_DIR}/CGAL-5.5.2/include)
        message(STATUS "The cgal lib is found!")
    else ()
        message(STATUS "The cgal lib is no found!")
    endif ()
endif ()
