cmake_minimum_required(VERSION 3.26)
project(BomberMan)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses)

if(NOT CURSES_FOUND AND WIN32)
    set(CURSES_INCLUDE_DIR "C:/MinGW/include")
    set(CURSES_LIBRARIES "C:/MinGW/lib/libpdcurses.a")
endif()

include_directories(${CURSES_INCLUDE_DIR})

add_executable(BomberMan
        main.cpp
        Bomb.cpp
        Bomb.hpp
        Character.cpp
        Character.hpp
        DummyEnemy.cpp
        DummyEnemy.hpp
        Game.cpp
        Game.hpp
        LevelManager.cpp
        LevelManager.hpp
        Map.cpp
        Map.hpp
        Player.cpp
        Player.hpp
        Renderer.cpp
        Renderer.hpp
        SmartEnemy.cpp
        SmartEnemy.hpp
)

target_link_libraries(BomberMan ${CURSES_LIBRARIES})