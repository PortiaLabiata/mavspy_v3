cmake_minimum_required(VERSION 3.13)
project(mavspyv3 LANGUAGES CXX)

add_compile_options(-Wno-address-of-packed-member -g)
add_executable(${PROJECT_NAME} 
    main.cpp
    capture_backend_ip.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    pcap)
