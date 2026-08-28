#include "app.hpp"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));

    App app;
    app.run();

    return 0;
}