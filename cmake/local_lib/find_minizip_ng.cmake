
# 添加minizip-ng
if (EXISTS ${3rdparty_dir}/minizip-ng)
    include_directories(${3rdparty_dir}/minizip-ng/include)
    link_directories(${3rdparty_dir}/minizip-ng/lib)

    set(MINIZIP_LIBS
            bzip2
            liblzma
            libminizip
            zlibstatic
            zstd_static)

    message(STATUS "The minizip_ng lib is found!")
else ()
    message(STATUS "The minizip_ng lib is no found!")
endif ()
