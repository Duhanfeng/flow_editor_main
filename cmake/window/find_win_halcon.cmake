
# 添加halcon
if (EXISTS $ENV{HALCONROOT})
    include_directories("$ENV{HALCONROOT}/include")
    include_directories("$ENV{HALCONROOT}/include/halconcpp")
    link_directories("$ENV{HALCONROOT}/lib/$ENV{HALCONARCH}")
    set(H_LIB "halcon" "halconcpp")
    message(STATUS "The halcon lib is found!")
else ()
    message(STATUS "The halcon lib is no found!")
endif ()
