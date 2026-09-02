#include "map.hpp"
#include "menu.hpp"
#include "renderer.hpp"

// Spazio minimo richiesto
// MIN_COLS: mappa + spazio (2) + info
// MIN_LINES: mappa (titolo non indispensabile)
const int MIN_COLS = MAP_WIDTH + 2 + INFO_WIDTH;
const int MIN_LINES = MAP_HEIGHT;

class App {
protected:
    bool running;
    Menu menu;

    bool terminal_too_small();

public:
    App();

    void run();
};