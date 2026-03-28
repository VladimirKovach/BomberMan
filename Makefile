ifeq ($(OS),Windows_NT)
    CURSES_LIB = -lpdcurses
else
    CURSES_LIB = -lncurses
endif

bomberman: main.o Character.o DummyEnemy.o Game.o Map.o Player.o Renderer.o Bomb.o LevelManager.o
	g++ main.o Character.o DummyEnemy.o Game.o Map.o Player.o Renderer.o Bomb.o LevelManager.o $(CURSES_LIB) -o bomberman

main.o: main.cpp Game.hpp
	g++ -c main.cpp

Character.o: Character.cpp Character.hpp Map.hpp
	g++ -c Character.cpp

DummyEnemy.o: DummyEnemy.cpp DummyEnemy.hpp
	g++ -c DummyEnemy.cpp

Game.o: Game.cpp Game.hpp LevelManager.hpp Map.hpp Player.hpp Renderer.hpp
	g++ -c Game.cpp

LevelManager.o: LevelManager.cpp LevelManager.hpp Map.hpp
	g++ -c LevelManager.cpp

Map.o: Map.cpp Map.hpp
	g++ -c Map.cpp

Player.o: Player.cpp Player.hpp
	g++ -c Player.cpp

Renderer.o: Renderer.cpp Renderer.hpp Map.hpp
	g++ -c Renderer.cpp

Bomb.o: Bomb.cpp Bomb.hpp Map.hpp
	g++ -c Bomb.cpp

clean:
	rm -f *.o
	rm -f bomberman