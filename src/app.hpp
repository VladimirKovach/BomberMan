#include "menu.hpp"

class App {
protected:
    bool running;
    Menu menu;

public:
    App();
    ~App();

    void run();
};