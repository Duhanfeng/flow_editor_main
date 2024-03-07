
# 添加spdlog
find_package(spdlog REQUIRED)
if (spdlog_FOUND)
    message(STATUS "The spdlog lib is found!")
else ()
    message(STATUS "The spdlog lib is no found!")
endif ()
