cmake_minimum_required(VERSION 3.26)
project(BomberMan)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses)

if(NOT CURSES_FOUND AND WIN32)
    set(CURSES_INCLUDE_DIR "C:/MinGW/include/ncurses")
    set(CURSES_LIBRARIES "C:/MinGW/lib/libncurses.a")
endif()

include_directories(${CURSES_INCLUDE_DIR})

add_executable(BomberMan
        main.cpp
        Bomb.cpp
        Character.cpp
        DummyEnemy.cpp
        Game.cpp
        Grid.cpp
        Leaderboard.cpp
        Level.cpp
        Map.cpp
        Menu.cpp
        Player.cpp
        Renderer.cpp
        SmartEnemy.cpp
        Item.cpp
)

target_link_libraries(BomberMan ${CURSES_LIBRARIES})