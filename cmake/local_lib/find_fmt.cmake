
# 添加fmt
#find_package(fmt REQUIRED)
#if (fmt_FOUND)
#    message(STATUS "The fmt lib is found!")
#else ()
#    message(STATUS "The fmt lib is no found!")
#endif ()

if (WIN32)
    if (EXISTS ${3rdparty_dir}/fmt)
        include_directories(${3rdparty_dir}/fmt/include)
        link_directories(${3rdparty_dir}/fmt/lib)
        message(STATUS "The fmt lib is found!")
    else ()
        message(STATUS "The fmt lib is no found!")
    endif ()
endif ()
