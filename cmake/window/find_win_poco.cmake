
 find_package(Poco REQUIRED COMPONENTS Foundation Zip)
 if (Poco_FOUND)
     message(STATUS "The Poco lib [${Poco_LIBRARIES}] is found!")
 else ()
     message(STATUS "The Poco lib is no found!")
 endif ()