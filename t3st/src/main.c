#include "engine.h"

int main(int argc, char** argv) {
    if (!engine_init("t3st-rts", 1280, 720)) return 1;
    engine_run();
    engine_shutdown();
    return 0;
}
