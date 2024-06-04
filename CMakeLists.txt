cmake_minimum_required(VERSION 3.5)

project(
    flphys
    DESCRIPTION "A library for calculating multi-body motion"
    LANGUAGES C
)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    flphys.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    C_STANDARD 99
)
