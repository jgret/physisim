#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <chrono> // time measurement
#include <vector>
#include "raylib.h"
#include "vector3f.h"
#include "rigidbody.h"
#include "cuboid.h"
#include "sphere.h"

#define frand() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::steady_clock::time_point timestamp;
#define getTimeDoubleMs(X) std::chrono::duration_cast<std::chrono::microseconds>(X).count() / 1000.0

// ------------- FUNCTION DEFINITIONS ---------------
static void update(float& fElapsedTime);
static void render(void);
static void UpdateDrawFrame(void);


// ------------- CONSTANTS --------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static const float camSense = 0.1f;
static const float camSpeed = 10.0f;
static const float maxTimeStep = 0.001f;
static const psim::Vector3f GRAVITY{ 0, -9.81f, 0 };


// ------------- VARIABLES --------------------------
static Font font = { 0 };
static Camera3D camera = { 0 };
static int nUpdateCount = 0;
static int nRequiredUpdateCount = 0;
static std::vector<psim::RigidBody*> bodies;
static double simulationTime = 0;
static psim::RigidBody* trackBody;


static void processInput(psim::Vector3f& camVel) {

    if (IsKeyDown(KEY_W)) {
        camVel.x = 1;
    }
    else if (IsKeyDown(KEY_S)) {
        camVel.x = -1;
    }
    else {
        camVel.x = 0;
    }

    if (IsKeyDown(KEY_A)) {
        camVel.y = -1;
    }
    else if (IsKeyDown(KEY_D)) {
        camVel.y = 1;
    }
    else {
        camVel.y = 0;
    }

    if (IsKeyDown(KEY_SPACE)) {
        camVel.z = 1;
    }
    else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        camVel.z = -1;
    }
    else {
        camVel.z = 0;
    }

    if (IsKeyPressed(KEY_F11)) {
        
        if (IsWindowFullscreen()) {
            
            SetWindowSize(screenWidth, screenHeight);

        }
        else {

            int display = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        }
        
        ToggleFullscreen();
    }
}


static void UpdateDrawFrame(void)
{
    float frameTime = GetFrameTime();

    timestamp timeStart = Clock::now();

    // mouse input
    Vector2 mouseDelta = GetMouseDelta();
    mouseDelta.x *= camSense;
    mouseDelta.y *= camSense;

    // camera movement
    psim::Vector3f camVel{0, 0, 0};
    processInput(camVel);
    camVel = camVel.normalize() * frameTime * camSpeed;
    
    UpdateCameraPro(&camera, camVel, psim::Vector3f{ mouseDelta.x, mouseDelta.y, 0 }, -GetMouseWheelMove());
    SetMousePosition(screenWidth / 2, screenHeight / 2);


    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
    {
        bodies[0]->getPos() = psim::Vector3f{ 0, 10, 0 };
        bodies[0]->getVel() = 0;
        bodies[0]->getAcc() = 0;
    }

    timestamp timeInput = Clock::now();


    float fPendingTime = frameTime;
    nUpdateCount = 0;
    nRequiredUpdateCount = (int) (frameTime / maxTimeStep);


    while (fPendingTime > 0) {

        float fElapsedTime;

        if (fPendingTime > maxTimeStep) {
            fElapsedTime = maxTimeStep;
        }
        else {
            fElapsedTime = fPendingTime;
        }

        update(fElapsedTime);
        simulationTime += fElapsedTime;
        nUpdateCount++;

        fPendingTime -= fElapsedTime;

    }

    timestamp timeUpdate = Clock::now();

    render();

    timestamp timeRender = Clock::now();

    double timeTaken = getTimeDoubleMs(timeUpdate - timeInput);
    double timeTakenAvg = nUpdateCount == 0 ? timeTaken : timeTaken / nUpdateCount;

    std::cout << "Timings" << std::endl;
    std::cout << " Input: " << std::fixed << getTimeDoubleMs(timeInput - timeStart) << "ms" << std::endl;
    std::cout << "Update: " << std::fixed << timeTaken << "ms" << std::endl;
    std::cout << "   avg: " << std::fixed << timeTakenAvg << "ms" << std::endl;
    std::cout << "Render: " << std::fixed << getTimeDoubleMs(timeRender - timeUpdate) << "ms" << std::endl;

    std::cout << std::endl;

}

static void update(float &fElapsedTime) {

    for (int cnt = 0; cnt < bodies.size(); cnt++) {

        psim::RigidBody *pObj1 = bodies[cnt];

        // clear acceleration
        pObj1->getAcc() = 0;

        for (int j = cnt + 1; j < bodies.size(); j++) {

            
            psim::RigidBody* pObj2 = bodies[j];
            
            if (pObj1->checkCollision(*pObj2)) {

                // reslove collision

                psim::Vector3f n = pObj2->getPos() - pObj1->getPos(); // axis on which velocity changes
                n = n.normalize();


                float v1bn = pObj1->getVel() * n.normalize();
                float v2bn = pObj2->getVel() * n.normalize();

                psim::Vector3f v1a; // new velocity for pBody
                psim::Vector3f v2a; // new velocity for pOther

                float m1 = pObj1->getMass();
                float m2 = pObj2->getMass();

                float e = 1;

                float v1an = ( m1 * v1bn + m2 * v2bn + m2 * e * (v2bn - v1bn) ) / ( m1 + m2 );
                float v2an = ( m1 * v1bn + m2 * v2bn - m2 * e * (v2bn - v1bn) ) / (m1 + m2);

                v1a = n * v1an;
                v2a = n * v2an;

                pObj1->getVel() = v1a;
                pObj2->getVel() = v2a;


            }
            else {
            }

        }

        // compute forces
        pObj1->addAcceleration(GRAVITY);

        // update physics
        pObj1->update(fElapsedTime);


        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            for (auto& body : bodies) {

                if (body->getShapeType() == psim::SPHERE) {
                    psim::Sphere &s = (psim::Sphere&) body->getShape();
                    Ray ray;
                    ray.direction = (psim::Vector3f{camera.target} - psim::Vector3f{camera.position}).normalize();
                    ray.position = psim::Vector3f{ camera.position };


                    RayCollision result = GetRayCollisionSphere(ray, s.getPos(), s.getRadius());
                
                    if (result.hit)
                    {
                        body->setColor(GREEN);
                        trackBody = body;
                    }
                    else {
                        body->setColor(BLUE);
                    }

                }
            }
        }


    }
}

static void render() {
    BeginDrawing();

        ClearBackground(LIGHTGRAY);

        BeginMode3D(camera);

        for (psim::RigidBody* body : bodies) {
            body->draw();
        }

        DrawGrid(100, 1.0f);

        EndMode3D();
        

        DrawFPS(10, 10);

        char buffer[400+1];
        sprintf(buffer, "Camera\nx = %.2f y = %.2f z = %.2f\nTarget\nx = %.2f y = %.2f z = %.2f", 
            camera.position.x, camera.position.y, camera.position.z,
            camera.target.x, camera.target.y, camera.target.z);
        DrawText(buffer, 10, 30, 5, BLACK);

        sprintf(buffer, "Update Count: %03d / %03d", nUpdateCount, nRequiredUpdateCount);
        DrawText(buffer, GetScreenWidth() - 125, 10, 5, RED);

        sprintf(buffer, "Simulation Time: %3.2f", simulationTime);
        DrawText(buffer, GetScreenWidth() - 125, 30, 5, BLACK);

        if (trackBody != nullptr) {
            sprintf(buffer, "RigidBody\npos { x: %2.2f y: %2.2f z: %2.2f }\nvel { x: %2.2f y: %2.2f z: %2.2f }\nacc { x: %2.2f y: %2.2f z: %2.2f }\n", trackBody->getPos().x, trackBody->getPos().y, trackBody->getPos().z, trackBody->getVel().x, trackBody->getVel().y, trackBody->getVel().z, trackBody->getAcc().x, trackBody->getAcc().y, trackBody->getAcc().z);
            DrawText(buffer, GetScreenWidth() - 200, 50, 3, BLACK);
        }

        int crosshairSize = 7;

        DrawRectangle(GetScreenWidth() / 2 - crosshairSize / 2, GetScreenHeight() / 2 - crosshairSize / 2, crosshairSize, crosshairSize, BLACK);

    EndDrawing();

}

int main(void)
{

    InitWindow(screenWidth, screenHeight, "physisim");
    
    font = LoadFont("resources/mecha.png");

	// init camera
	camera.position = psim::Vector3f{ 0.0f, 5.0f, 10.0f };
	camera.target = psim::Vector3f{ 0.0f, 0.0f, 0.0f };
	camera.up = psim::Vector3f{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;


    SetTargetFPS(144);
    HideCursor();

    simulationTime = 0;

    // add rectangle
    psim::RigidBody* a = new psim::RigidBody(new psim::Sphere(psim::Vector3f{0, 1, 0}, 1));
    psim::RigidBody* b = new psim::RigidBody(new psim::Sphere(psim::Vector3f{0, 10, 0}, 1));

    for (int i = 0; i < 10; i++) {

        psim::Vector3f pos {
            10 * frand() - 5,
            (float) i * 2 + 20,
            10 * frand() - 5
        };

        psim::RigidBody* c = new psim::RigidBody(new psim::Sphere(psim::Vector3f{pos}, 1));

        
        bodies.push_back(c);


    }

    bodies.push_back(a);
    bodies.push_back(b);

    while (!WindowShouldClose()) // window close or esc button
    {
        UpdateDrawFrame();
    }

    UnloadFont(font);

    CloseWindow();
    return 0;
}