
# 添加spdlog
if (WIN32)
    if (EXISTS ${CUSTOM_LIB_DIR}/spdlog)
        include_directories(${CUSTOM_LIB_DIR}/spdlog/include)
        link_directories(${CUSTOM_LIB_DIR}/spdlog/lib)
        message(STATUS "The spdlog lib is found!")
    else ()
        message(STATUS "The spdlog lib is no found!")
    endif ()
endif ()
