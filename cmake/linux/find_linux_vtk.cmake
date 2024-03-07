
find_package(VTK REQUIRED)
if (VTK_FOUND)
    message(STATUS "The vtk lib is found!")
else ()
    message(STATUS "The vtk lib is no found!")
endif ()
