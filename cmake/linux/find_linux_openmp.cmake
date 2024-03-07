
# 添加OpenMP
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    message(STATUS "The OpenMP lib [${OpenMP_C_FLAGS}, ${OpenMP_CXX_FLAGS}, ${OpenMP_EXE_LINKER_FLAGS}] is found!")
else ()
    message(STATUS "The OpenMP lib is no found!")
endif ()
