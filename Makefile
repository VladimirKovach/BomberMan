bomberman: main.o app.o bomb.o chaser.o enemy.o game.o item.o leaderboard.o level_manager.o level.o map.o menu.o player.o position.o renderer.o roamer.o walker.o
	g++ main.o app.o bomb.o chaser.o enemy.o game.o item.o leaderboard.o level_manager.o level.o map.o menu.o player.o position.o renderer.o roamer.o walker.o -lncurses -o bomberman

main.o: src/main.cpp src/app.hpp
	g++ -c src/main.cpp

app.o: src/app.cpp src/app.hpp src/game.hpp src/menu.hpp
	g++ -c src/app.cpp

bomb.o: src/bomb.cpp src/bomb.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/bomb.cpp

chaser.o: src/chaser.cpp src/chaser.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/chaser.cpp

enemy.o: src/enemy.cpp src/enemy.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/enemy.cpp

game.o: src/game.cpp src/game.hpp src/level_manager.hpp src/level.hpp src/player.hpp src/renderer.hpp src/position.hpp src/time.hpp
	g++ -c src/game.cpp

item.o: src/item.cpp src/item.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/item.cpp

leaderboard.o: src/leaderboard.cpp src/leaderboard.hpp
	g++ -c src/leaderboard.cpp

level_manager.o: src/level_manager.cpp src/level_manager.hpp src/level.hpp
	g++ -c src/level_manager.cpp

level.o: src/level.cpp src/level.hpp src/bomb.hpp src/chaser.hpp src/item.hpp src/map.hpp src/position.hpp src/roamer.hpp src/walker.hpp
	g++ -c src/level.cpp

map.o: src/map.cpp src/map.hpp src/position.hpp
	g++ -c src/map.cpp

menu.o: src/menu.cpp src/menu.hpp src/leaderboard.hpp
	g++ -c src/menu.cpp

player.o: src/player.cpp src/player.hpp src/item.hpp src/map.hpp src/position.hpp
	g++ -c src/player.cpp

position.o: src/position.cpp src/position.hpp
	g++ -c src/position.cpp

renderer.o: src/renderer.cpp src/renderer.hpp src/bomb.hpp src/item.hpp src/level_manager.hpp src/level.hpp src/player.hpp src/position.hpp
	g++ -c src/renderer.cpp

roamer.o: src/roamer.cpp src/roamer.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/roamer.cpp

walker.o: src/walker.cpp src/walker.hpp src/map.hpp src/position.hpp src/time.hpp
	g++ -c src/walker.cpp

clean:
	rm -f bomberman *.o