
# 添加nlohmann
if (WIN32)
    if (EXISTS ${CUSTOM_LIB_DIR}/nlohmann)
        include_directories(${CUSTOM_LIB_DIR})
        include_directories(${CUSTOM_LIB_DIR}/nlohmann)
        message(STATUS "The nlohmann lib is found!")
    else ()
        message(STATUS "The nlohmann lib is no found!")
    endif ()
endif ()
