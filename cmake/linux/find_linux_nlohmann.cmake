
# 添加nlohmann
find_package(nlohmann_json REQUIRED)
if (nlohmann_json_FOUND)
    message(STATUS "The nlohmann lib is found!")
else ()
    message(STATUS "The nlohmann lib is no found!")
endif ()
