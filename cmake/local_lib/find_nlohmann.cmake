
# 添加nlohmann
if (WIN32)
    if (EXISTS ${3rdparty_dir}/nlohmann)
        include_directories(${3rdparty_dir})
        include_directories(${3rdparty_dir}/nlohmann)
        message(STATUS "The nlohmann lib is found!")
    else ()
        message(STATUS "The nlohmann lib is no found!")
    endif ()
endif ()
