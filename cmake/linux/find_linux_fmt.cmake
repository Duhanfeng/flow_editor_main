
# 添加fmt
find_package(fmt REQUIRED)
if (fmt_FOUND)
    message(STATUS "The fmt lib is found!")
else ()
    message(STATUS "The fmt lib is no found!")
endif ()
