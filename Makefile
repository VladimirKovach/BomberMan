bomberman: main.o Bomb.o DummyEnemy.o Game.o Grid.o Item.o Leaderboard.o Level.o Map.o Menu.o Player.o Renderer.o SmartEnemy.o
	g++ main.o Bomb.o DummyEnemy.o Game.o Grid.o Item.o Leaderboard.o Level.o Map.o Menu.o Player.o Renderer.o SmartEnemy.o -lncurses -o bomberman

main.o: main.cpp Game.hpp
	g++ -c main.cpp

Bomb.o: Bomb.cpp Bomb.hpp Grid.hpp
	g++ -c Bomb.cpp

DummyEnemy.o: DummyEnemy.cpp DummyEnemy.hpp
	g++ -c DummyEnemy.cpp

Game.o: Game.cpp Game.hpp Map.hpp Player.hpp Renderer.hpp
	g++ -c Game.cpp

Grid.o: Grid.cpp Grid.hpp
	g++ -c Grid.cpp

Item.o: Item.cpp Item.hpp Grid.hpp
	g++ -c Item.cpp

Leaderboard.o: Leaderboard.cpp Leaderboard.hpp
	g++ -c Leaderboard.cpp

Level.o: Level.cpp Level.hpp Grid.hpp Bomb.hpp DummyEnemy.hpp SmartEnemy.hpp Item.hpp
	g++ -c Level.cpp

Map.o: Map.cpp Map.hpp Level.hpp
	g++ -c Map.cpp

Menu.o: Menu.cpp Menu.hpp
	g++ -c Menu.cpp

Player.o: Player.cpp Player.hpp Item.hpp
	g++ -c Player.cpp

Renderer.o: Renderer.cpp Renderer.hpp Map.hpp
	g++ -c Renderer.cpp

SmartEnemy.o: SmartEnemy.cpp SmartEnemy.hpp
	g++ -c SmartEnemy.cpp

clean:
	rm -f *.o
	rm -f bomberman