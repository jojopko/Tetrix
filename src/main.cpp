#include "game.h"
#include "global.h"

int main(int argc, char ** argv) {
    if (init_game()) return 1;
    run_game();
    return 0;
}

