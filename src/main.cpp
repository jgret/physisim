#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>

#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "physisim.h"

static const int screenWidth = 1280;
static const int screenHeight = 720;

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
            std::cout << "Restarting Simulation..." << std::endl;
            delete simulation;
            simulation = new psim::Simulation();
            simulation->init();
        }
    }

    UnloadFont(font);

    CloseWindow();
    return 0;
}