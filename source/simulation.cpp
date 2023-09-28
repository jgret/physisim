#include <iostream>
#include "simulation.h"
#include "utils.h"
#include "sphere.h"

// ------------- CONSTANTS --------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static const float camSense = 0.1f;
static const float camSpeed = 10.0f;
static const float maxTimeStep = 0.001f;
static const psim::Vector3f GRAVITY{ 0, -9.81f, 0 };

psim::Simulation::Simulation()
{
	trackBody = nullptr;
	simulationTime = 0;
	camera = { 0 };
	nUpdateCount = 0;
	nRequiredUpdateCount = 0;
}

bool psim::Simulation::init()
{
	// init camera
	camera.position = psim::Vector3f{ 0.0f, 5.0f, 15.0f };
	camera.target = psim::Vector3f{ 0.0f, 4.0f, 0.0f };
	camera.up = psim::Vector3f{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	// add objects
	psim::RigidBody* a = new psim::RigidBody(new psim::Sphere(psim::Vector3f{1, 1, 0}, 1));
	psim::RigidBody* b = new psim::RigidBody(new psim::Sphere(psim::Vector3f{0, 10, 0}, 1));
	system.addRigidBody(a);
	system.addRigidBody(b);
	a->setRestitution(0.7);
	b->setRestitution(0.7);

	return true;
}


void psim::Simulation::update(float fElapsedTime)
{

	system.clearForces();
	system.checkCollision();
	system.applyGravity();
	system.step(fElapsedTime);

}

void psim::Simulation::render()
{
	BeginDrawing();

	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);

	for (psim::RigidBody* body : system.getObjects())
	{
		body->draw();
	}

	DrawGrid(100, 1.0f);

	EndMode3D();


	DrawFPS(10, 10);

	char buffer[400 + 1];
	sprintf(buffer, "Camera\nx = %.2f y = %.2f z = %.2f\nTarget\nx = %.2f y = %.2f z = %.2f",
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z);
	DrawText(buffer, 10, 30, 5, BLACK);

	sprintf(buffer, "Update Count: %03d / %03d", nUpdateCount, nRequiredUpdateCount);
	DrawText(buffer, GetScreenWidth() - 125, 10, 5, RED);

	sprintf(buffer, "Simulation Time: %3.2f", simulationTime);
	DrawText(buffer, GetScreenWidth() - 125, 30, 5, BLACK);

	sprintf(buffer, "Energy: %3.2f", system.getTotalEnergy());
	DrawText(buffer, GetScreenWidth() - 125, 50, 5, PURPLE);

	if (trackBody != nullptr)
	{
		sprintf(buffer, "RigidBody\npos { x: %2.2f y: %2.2f z: %2.2f }\nvel { x: %2.2f y: %2.2f z: %2.2f }\nacc { x: %2.2f y: %2.2f z: %2.2f }\n", trackBody->getPos().x, trackBody->getPos().y, trackBody->getPos().z, trackBody->getVel().x, trackBody->getVel().y, trackBody->getVel().z, trackBody->getAcc().x, trackBody->getAcc().y, trackBody->getAcc().z);
		DrawText(buffer, GetScreenWidth() - 200, 50, 3, BLACK);
	}

	int crosshairSize = 7;

	DrawRectangle(GetScreenWidth() / 2 - crosshairSize / 2, GetScreenHeight() / 2 - crosshairSize / 2, crosshairSize, crosshairSize, BLACK);

	EndDrawing();
}

bool psim::Simulation::pause()
{
	return false;
}

bool psim::Simulation::run()
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

	UpdateCameraPro(&camera, camVel, psim::Vector3f{ mouseDelta.x, mouseDelta.y, 0 }, 0);
	SetMousePosition(screenWidth / 2, screenHeight / 2);


	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{

		psim::RigidBody* body = system.getObjects().at(0);

		body->getPos() = psim::Vector3f{ 0, 10, 0 };
		body->getVel() = 0;
		body->getAcc() = 0;
	}

	timestamp timeInput = Clock::now();


	float fPendingTime = frameTime;
	nUpdateCount = 0;
	nRequiredUpdateCount = (int)(frameTime / maxTimeStep);


	while (fPendingTime > 0)
	{

		float fElapsedTime;

		if (fPendingTime > maxTimeStep)
		{
			fElapsedTime = maxTimeStep;
		}
		else
		{
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

	//std::cout << "Timings" << std::endl;
	//std::cout << " Input: " << std::fixed << getTimeDoubleMs(timeInput - timeStart) << "ms" << std::endl;
	//std::cout << "Update: " << std::fixed << timeTaken << "ms" << std::endl;
	//std::cout << "   avg: " << std::fixed << timeTakenAvg << "ms" << std::endl;
	//std::cout << "Render: " << std::fixed << getTimeDoubleMs(timeRender - timeUpdate) << "ms" << std::endl;
	//std::cout << std::endl;

	return true;
}

void psim::Simulation::processInput(psim::Vector3f& camVel)
{
	if (IsKeyDown(KEY_W))
	{
		camVel.x = 1;
	}
	else if (IsKeyDown(KEY_S))
	{
		camVel.x = -1;
	}
	else
	{
		camVel.x = 0;
	}

	if (IsKeyDown(KEY_A))
	{
		camVel.y = -1;
	}
	else if (IsKeyDown(KEY_D))
	{
		camVel.y = 1;
	}
	else
	{
		camVel.y = 0;
	}

	if (IsKeyDown(KEY_SPACE))
	{
		camVel.z = 1;
	}
	else if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		camVel.z = -1;
	}
	else
	{
		camVel.z = 0;
	}

	if (IsKeyPressed(KEY_F11))
	{
		toggleFullScreen();
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		Ray ray;
		ray.direction = (psim::Vector3f{camera.target} - psim::Vector3f{camera.position}).normalize();
		ray.position = psim::Vector3f{ camera.position };
		trackBody = system.raycastSelect(ray);
	}
}

void psim::Simulation::toggleFullScreen()
{
	if (IsWindowFullscreen())
	{

		ToggleFullscreen();
		SetWindowSize(screenWidth, screenHeight);
	}
	else
	{

		int display = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
		ToggleFullscreen();
	}
}
