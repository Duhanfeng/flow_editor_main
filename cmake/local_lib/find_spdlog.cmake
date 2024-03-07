
# 添加spdlog
if (WIN32)
    if (EXISTS ${3rdparty_dir}/spdlog)
        include_directories(${3rdparty_dir}/spdlog/include)
        link_directories(${3rdparty_dir}/spdlog/lib)
        message(STATUS "The spdlog lib is found!")
    else ()
        message(STATUS "The spdlog lib is no found!")
    endif ()
endif ()
