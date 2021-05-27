project(oledLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src src)

add_library(oledLib STATIC ${src})

find_package(wiringPi REQUIRED)
include_directories(${WIRINGPI_INCLUDE_DIRS}) 
target_link_libraries(oledLib ${WIRINGPI_LIBRARIES})