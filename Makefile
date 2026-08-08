bomberman: main.o bomb.o enemy.o game.o item.o leaderboard.o level_manager.o level.o map.o menu.o player.o renderer.o utils.o
	g++ main.o bomb.o enemy.o game.o item.o leaderboard.o level_manager.o level.o map.o menu.o player.o renderer.o utils.o -lncurses -o bomberman
main.o: src/main.cpp src/game.hpp src/menu.hpp
	g++ -c src/main.cpp

bomb.o: src/bomb.cpp src/bomb.hpp src/map.hpp src/player.hpp src/utils.hpp
	g++ -c src/bomb.cpp

enemy.o: src/enemy.cpp src/enemy.hpp src/map.hpp src/utils.hpp
	g++ -c src/enemy.cpp

game.o: src/game.cpp src/game.hpp src/level_manager.hpp src/player.hpp src/renderer.hpp src/utils.hpp
	g++ -c src/game.cpp

item.o: src/item.cpp src/item.hpp src/map.hpp src/utils.hpp
	g++ -c src/item.cpp

leaderboard.o: src/leaderboard.cpp src/leaderboard.hpp
	g++ -c src/leaderboard.cpp

level_manager.o: src/level_manager.cpp src/level_manager.hpp src/level.hpp
	g++ -c src/level_manager.cpp

level.o: src/level.cpp src/level.hpp src/bomb.hpp src/enemy.hpp src/map.hpp src/utils.hpp
	g++ -c src/level.cpp

map.o: src/map.cpp src/map.hpp src/utils.hpp
	g++ -c src/map.cpp

menu.o: src/menu.cpp src/menu.hpp src/leaderboard.hpp
	g++ -c src/menu.cpp

player.o: src/player.cpp src/player.hpp src/map.hpp src/utils.hpp
	g++ -c src/player.cpp

renderer.o: src/renderer.cpp src/renderer.hpp src/level_manager.hpp src/player.hpp src/utils.hpp
	g++ -c src/renderer.cpp

utils.o: src/utils.cpp src/utils.hpp
	g++ -c src/utils.cpp

clean:
	rm -f *.o
	rm -f bomberman