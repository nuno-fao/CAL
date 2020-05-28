cmake_minimum_required(VERSION 3.15)
project(CAL_PROJ)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
include_directories(mapas)
link_libraries(ws2_32 wsock32)

add_executable(CAL_PROJ
        lib/connection.cpp
        lib/connection.h
        lib/edgetype.h
        lib/Graph.h
        lib/GraphFuncs.cpp
        lib/GraphFuncs.h
        lib/graphviewer.cpp
        lib/graphviewer.h
        lib/MutablePriorityQueue.h
        lib/Node.h
        main.cpp lib/GraphViewerFuncs.h lib/GraphViewerFuncs.cpp lib/Vehicle.h lib/Vehicle.cpp lib/Service.h lib/Service.cpp lib/Menus.h lib/Menus.cpp)
