cmake_minimum_required(VERSION 3.13)
project(mavspyv3 LANGUAGES CXX)

add_compile_options(-Wno-address-of-packed-member -g)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui
    imgui.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp)

add_executable(${PROJECT_NAME} 
    main.cpp
    capture_backend_ip.cpp
    gui_backend.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    pcap glfw3 GL imgui)
