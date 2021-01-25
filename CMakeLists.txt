cmake_minimum_required(VERSION 3.1)

set(TARGET_OS \"${CMAKE_SYSTEM_NAME}\")

set(PROJECT_NAME "FLTKExtraWidgets${CMAKE_SYSTEM_NAME}" )

project(${PROJECT_NAME})

FILE(GLOB SRCS "src/*.cpp")

#message("TARGET_OS = '${CMAKE_SYSTEM_NAME}'")
add_library(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE "include")

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install (FILES "include/Widgets.h"
        DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake
)
