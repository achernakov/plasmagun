set (SOURCES_LIST src/main.cpp src/net/socket.cpp
		src/gui/window.cpp src/gui/main_window.cpp 
		src/net/connection.cpp src/infiniium/infiniium_connection.cpp
		src/gui/oscope_conn.cpp src/infiniium/infiniium_proto.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--export-dynamic -rdynamic -std=c++11 -pthread")
project(plasmagun)
cmake_minimum_required(VERSION 2.6)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTK3 REQUIRED gmodule-export-2.0)
	

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
add_executable(plasmagun ${SOURCES_LIST})
target_link_libraries(plasmagun ${GTK3_LIBRARIES})

add_executable(test_plasma src/test_main.cpp src/net/socket.cpp)
