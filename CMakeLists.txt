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
        src/main.cpp
        src/bomb.cpp
        src/dummy_enemy.cpp
        src/game.cpp
        src/item.cpp
        src/leaderboard.cpp
        src/level.cpp
        src/level_manager.cpp
        src/map.cpp
        src/menu.cpp
        src/player.cpp
        src/renderer.cpp
        src/smart_enemy.cpp
        src/utils.cpp
)

target_link_libraries(BomberMan ${CURSES_LIBRARIES})