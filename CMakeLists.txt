cmake_minimum_required(VERSION 3.15)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MINIMUM_CXX_VERSION 20)
set(MINIMUM_CXX_VERSION_REQUIRED on)

project(dbreaderv2)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Boost REQUIRED)

add_executable(dbreaderv2 main.cpp

    table.hpp table.cpp table.ui
    opendb.hpp opendb.cpp opendb.ui
)

target_link_libraries(dbreaderv2 Qt6::Widgets Qt6::Core Qt6::Sql)

target_include_directories(dbreaderv2 PUBLIC ${Boost_INCLUDE_DIRS})
