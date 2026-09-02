cmake_minimum_required(VERSION 3.26)
project(BomberMan)

set(CMAKE_CXX_STANDARD 17)

# Warning estesi
add_compile_options(-Wall -Wextra)

find_package(Curses)

if(NOT CURSES_FOUND AND WIN32)
    set(CURSES_INCLUDE_DIR "C:/MinGW/include/ncurses")
    set(CURSES_LIBRARIES "C:/MinGW/lib/libncurses.a")
endif()

include_directories(${CURSES_INCLUDE_DIR})

add_executable(BomberMan
        src/main.cpp
        src/app.cpp
        src/bomb.cpp
        src/chaser.cpp
        src/enemy.cpp
        src/game.cpp
        src/item.cpp
        src/leaderboard.cpp
        src/level_manager.cpp
        src/level.cpp
        src/map.cpp
        src/menu.cpp
        src/player.cpp
        src/position.cpp
        src/renderer.cpp
        src/roamer.cpp
        src/walker.cpp
)

target_link_libraries(BomberMan ${CURSES_LIBRARIES})