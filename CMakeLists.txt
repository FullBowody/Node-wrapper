cmake_minimum_required(VERSION 3.15)
project(NodeWrapper)

file(GLOB NodeWrapper_SRC
    "src/*.hpp"
    "src/*.cpp"
)

add_executable(NodeWrapper ${NodeWrapper_SRC})
target_include_directories(NodeWrapper PUBLIC "include")
target_link_directories(NodeWrapper PUBLIC "lib")
