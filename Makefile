bomberman: main.o Bomb.o DummyEnemy.o Game.o Leaderboard.o Level.o LevelManager.o Map.o Menu.o Player.o Renderer.o SmartEnemy.o
	g++ main.o Bomb.o DummyEnemy.o Game.o Leaderboard.o Level.o LevelManager.o Map.o Menu.o Player.o Renderer.o SmartEnemy.o -lncurses -o bomberman

main.o: main.cpp Game.hpp
	g++ -c main.cpp

Bomb.o: Bomb.cpp Bomb.hpp Map.hpp
	g++ -c Bomb.cpp

DummyEnemy.o: DummyEnemy.cpp DummyEnemy.hpp
	g++ -c DummyEnemy.cpp

Game.o: Game.cpp Game.hpp LevelManager.hpp Player.hpp Renderer.hpp
	g++ -c Game.cpp

Map.o: Map.cpp Map.hpp
	g++ -c Map.cpp

Leaderboard.o: Leaderboard.cpp Leaderboard.hpp
	g++ -c Leaderboard.cpp

Level.o: Level.cpp Level.hpp Map.hpp Bomb.hpp DummyEnemy.hpp SmartEnemy.hpp
	g++ -c Level.cpp

LevelManager.o: LevelManager.cpp LevelManager.hpp Level.hpp
	g++ -c LevelManager.cpp

Menu.o: Menu.cpp Menu.hpp
	g++ -c Menu.cpp

Player.o: Player.cpp Player.hpp
	g++ -c Player.cpp

Renderer.o: Renderer.cpp Renderer.hpp LevelManager.hpp
	g++ -c Renderer.cpp

SmartEnemy.o: SmartEnemy.cpp SmartEnemy.hpp
	g++ -c SmartEnemy.cpp

clean:
	rm -f *.o
	rm -f bomberman