cmake_minimum_required(VERSION 3.18)

project(SmartButlerForRaspberry)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    set(CMAKE_C_FLAGS "-no-pie -std=c17 -Wall -Werror -Wextra -pedantic -Wimplicit-fallthrough -Wsequence-point -Wswitch-default -Wswitch-unreachable -Wswitch-enum -Wstringop-truncation -Wbool-compare -Wtautological-compare -Wfloat-equal -Wshadow=global -Wpointer-arith -Wpointer-compare -Wcast-align -Wcast-qual -Wwrite-strings -Wdangling-else -Wlogical-op -Wconversion -Wno-pedantic-ms-format")

    set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -D DEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-o2")
    
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/modules)

    add_subdirectory(${CMAKE_SOURCE_DIR}/modules)

    include_directories(${CMAKE_SOURCE_DIR}/modules/oledLib/include)
    include_directories(${CMAKE_SOURCE_DIR}/modules/getInformation/include)

    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
    aux_source_directory(${CMAKE_SOURCE_DIR} src)
    add_executable(.SmartButlerForRaspberry ${src})

    target_link_libraries(.SmartButlerForRaspberry oledLib getInformation)

    install(TARGETS .SmartButlerForRaspberry DESTINATION /home/pi/)
    install(PROGRAMS SmartButlerForRaspberry.desktop DESTINATION /home/pi/.config/autostart/)
else ()
    message(FATAL_ERROR "不支持的编译环境")
endif ()