cmake_minimum_required(VERSION 3.21)
project(suffixTree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(.idea)

add_executable(suffixTree
        .idea/.gitignore
        .idea/modules.xml
        .idea/suffixTree.iml
        .idea/vcs.xml
        .idea/workspace.xml
        main.cpp)
