
# 添加review_glqt
if (EXISTS ${3rdparty_dir}/review_glqt)
    include_directories(${3rdparty_dir}/review_glqt/include)
    link_directories(${3rdparty_dir}/review_glqt/lib)

    message(STATUS "The review_glqt lib is found!")
else ()
    message(STATUS "The review_glqt lib is no found!")
endif ()
