add_library(FLTKExtraWidgets "${CMAKE_CURRENT_LIST_DIR}/src/Widgets.cpp")
target_include_directories(FLTKExtraWidgets PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_compile_definitions(FLTKExtraWidgets PUBLIC -D_WINDOWS_)
endif()
