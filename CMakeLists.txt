cmake_minimum_required(VERSION 3.12)

project(OpenGLTestFramework LANGUAGES CXX)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(Application)
add_subdirectory(glinterface)
add_subdirectory(thirdparty/gdstk)