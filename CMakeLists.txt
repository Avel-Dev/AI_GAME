cmake_minimum_required(VERSION 3.10)
project(MyRaylibGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(raylib)
# Create executable
add_executable(MyRaylibGame 
	main.cpp)

# Link raylib
target_link_libraries(MyRaylibGame raylib)

# (Optional) For Linux: link additional libraries if needed
if(UNIX AND NOT APPLE)
    target_link_libraries(MyRaylibGame m pthread dl rt X11)
endif()
