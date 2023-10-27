cmake_minimum_required(VERSION 3.24)
project(Server_ProyectoII)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server_ProyectoII main.cpp)

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
target_link_libraries(Server_ProyectoII ${GTK3_LIBRARIES})
target_link_libraries(Server_ProyectoII ${GTK3_LIBRARIES} SDL2)