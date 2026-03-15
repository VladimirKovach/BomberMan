bomberman: main.o Character.o Enemy.o Game.o Map.o Player.o Renderer.o
	g++ main.o Character.o Enemy.o Game.o Map.o Player.o Renderer.o -lncurses -o bomberman

main.o: main.cpp Game.hpp
	g++ -c main.cpp

Character.o: Character.cpp Character.hpp Map.hpp
	g++ -c Character.cpp

Enemy.o: Enemy.cpp Enemy.hpp
	g++ -c Enemy.cpp

Game.o: Game.cpp Game.hpp Map.hpp Player.hpp Renderer.hpp
	g++ -c Game.cpp

Map.o: Map.cpp Map.hpp
	g++ -c Map.cpp

Player.o: Player.cpp Player.hpp
	g++ -c Player.cpp

Renderer.o: Renderer.cpp Renderer.hpp Map.hpp
	g++ -c Renderer.cpp

clean:
	rm -f *.o
	rm -f bomberman