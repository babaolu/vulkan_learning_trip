cmake_minimum_required(VERSION 3.20)

project(VulkanTest)

add_executable(VulkanTest VFirst.cpp)

target_link_libraries(VulkanTest -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi)
