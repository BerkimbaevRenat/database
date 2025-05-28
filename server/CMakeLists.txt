cmake_minimum_required(VERSION 3.28)
project(server)

set(CMAKE_CXX_STANDARD 23)

add_executable(server main.cpp
        Schedule/Schedule.h
        Schedule/Schedule.cpp
        tools/tools.cpp
        tools/tools.hpp
        Schedule/Select.cpp
        AnalyseQuery/AnalyseQuery.h
        AnalyseQuery/AnalyseQuery.cpp
        Schedule/ImportExport.cpp
)
