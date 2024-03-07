
# 添加fmt
#find_package(fmt REQUIRED)
#if (fmt_FOUND)
#    message(STATUS "The fmt lib is found!")
#else ()
#    message(STATUS "The fmt lib is no found!")
#endif ()

if (WIN32)
    if (EXISTS ${CUSTOM_LIB_DIR}/fmt)
        include_directories(${CUSTOM_LIB_DIR}/fmt/include)
        link_directories(${CUSTOM_LIB_DIR}/fmt/lib)
        message(STATUS "The fmt lib is found!")
    else ()
        message(STATUS "The fmt lib is no found!")
    endif ()
endif ()
