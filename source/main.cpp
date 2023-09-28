#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>

#include <vector>
#include "raylib.h"
#include "physisim.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

static Font font = { 0 };
static psim::Simulation *simulation;

int main(void)
{

    InitWindow(screenWidth, screenHeight, "physisim");
    SetTargetFPS(144);
    HideCursor();

    font = LoadFont("resources/mecha.png");
    simulation = new psim::Simulation();
    simulation->init();

    while (!WindowShouldClose()) // window close or esc button
    {
        simulation->run();

        if (IsKeyPressed(KEY_R))
        {
            // restart simulation;
            delete simulation;
            simulation = new psim::Simulation();
            simulation->init();
        }
    }

    UnloadFont(font);

    CloseWindow();
    return 0;
}