#include <iostream>
#include "simulation.h"
#include "utils.h"
#include "sphere.h"
#include "raymath.h"

// ------------- CONSTANTS --------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static const float camSense = 0.1f;
static const float camSpeed = 10.0f;
static const float maxTimeStep = 0.001f;
static const psim::Vector3f GRAVITY{ 0, -9.81f, 0 };

static Model model1;
static psim::Vector3f rotation{0, 0, 0};

psim::Simulation::Simulation()
{
	bPaused = false;
	trackBody = nullptr;
	springBody = nullptr;
	infoBody = nullptr;
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
	static const int object_count = 20;
	for (int i = 0; i < object_count; i++)
	{
		psim::RigidBody* body = new psim::RigidBody(new psim::Sphere(
			psim::Vector3f
			{
				frand() * 10 - 5,
				(float) i * 2,
				frand() * 10 - 5
			}, frand() * 1 + 1
		));
		body->setRestitution(0.7);
		system.addRigidBody(body);
	}

	psim::RigidBody* a = new psim::RigidBody(new psim::Sphere(psim::Vector3f{0, 1, 0}, 1));
	psim::RigidBody* b = new psim::RigidBody(new psim::Sphere(psim::Vector3f{-5, 1, 0}, 1));
	system.addRigidBody(a);
	system.addRigidBody(b);
	a->setRestitution(0.7);
	b->setRestitution(0.7);


	b->getVel() = psim::Vector3f(1, 0, 0);

	//psim::RigidBody* a = new psim::RigidBody(new psim::Sphere(psim::Vector3f{1, 1, 0}, 1));
	//psim::RigidBody* b = new psim::RigidBody(new psim::Sphere(psim::Vector3f{0, 10, 0}, 1));
	//a->setRestitution(0.7);
	//b->setRestitution(0.7);
	//system.addRigidBody(a);
	//system.addRigidBody(b);


	Mesh cube1 = GenMeshCube(1, 1, 1);
	model1 = LoadModelFromMesh(cube1);

	return true;
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


	timestamp timeInput = Clock::now();

	if (!bPaused)
	{
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

	if (IsKeyPressed(KEY_P))
	{
		this->bPaused = !bPaused;
	}


	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (springBody == nullptr)
		{
			Ray ray{ 0 };
			ray.direction = (psim::Vector3f{camera.target} - psim::Vector3f{camera.position}).normalize();
			ray.position = psim::Vector3f{ camera.position };
			springBody = system.raycastSelect(ray);
			if (springBody)
			{
				springBody->setColor(RED);
				springBody->setDamping(SPRING_DAMPING);
			}
		}
		else
		{
			springBody->setColor(BLUE);
			springBody->setDamping(AIR_DAMPING);
			springBody = nullptr;
		}
	} 
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{

		if (trackBody == nullptr)
		{
			Ray ray{ 0 };
			ray.direction = (psim::Vector3f{camera.target} - psim::Vector3f{camera.position}).normalize();
			ray.position = psim::Vector3f{ camera.position };
			trackBody = system.raycastSelect(ray);
			if (trackBody)
				trackBody->setColor(GREEN);
		}
		else
		{
			trackBody->setColor(BLUE);
			trackBody = nullptr;
		}
 
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
	{

		if (infoBody == nullptr)
		{
			Ray ray{ 0 };
			ray.direction = (psim::Vector3f{camera.target} - psim::Vector3f{camera.position}).normalize();
			ray.position = psim::Vector3f{ camera.position };
			infoBody = system.raycastSelect(ray);
			if (infoBody)
				infoBody->setColor(YELLOW);
		}
		else
		{
			infoBody->setColor(BLUE);
			infoBody = nullptr;
		}

	}
}

void psim::Simulation::update(float fElapsedTime)
{

	system.clearForces();
	system.checkCollision();

	if (trackBody)
	{
		trackBody->getPos() = camera.target;
		trackBody->getVel() = Vector3f::ZERO;
	}

	if (springBody)
	{
		Vector3f diff = (Vector3f{ camera.target } - springBody->getPos());
		Vector3f dir = diff.normalize();

		float kx = 10;

		Vector3f springForce = diff * kx;
		springBody->applyForce(springForce);

	}

	system.applyGravity();
	system.step(fElapsedTime);


}

void psim::Simulation::render()
{
	BeginDrawing();

	ClearBackground(WHITE);

	BeginMode3D(camera);

		for (psim::RigidBody* body : system.getObjects())
		{
			body->draw();
		}

		if (springBody)
		{
			DrawLine3D(camera.target, springBody->getPos(), BLACK);
		}

		//rotation.x += 0.01;
		//rotation.y += 0.02;
		//model1.transform = MatrixRotateXYZ(rotation);

		//DrawModel(model1, psim::Vector3f{1, 1, 1}, 3, GREEN);
	    //DrawModelWires(model1, psim::Vector3f{1, 1, 1}, 3, BLACK);

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

	if (infoBody != nullptr)
	{
		sprintf(buffer, "RigidBody\npos { x: %2.2f y: %2.2f z: %2.2f }\nvel { x: %2.2f y: %2.2f z: %2.2f }\nacc { x: %2.2f y: %2.2f z: %2.2f }\n", infoBody->getPos().x, infoBody->getPos().y, infoBody->getPos().z, infoBody->getVel().x, infoBody->getVel().y, infoBody->getVel().z, infoBody->getAcc().x, infoBody->getAcc().y, infoBody->getAcc().z);
		DrawText(buffer, GetScreenWidth() - 200, 50, 3, BLACK);
	}

	if (bPaused)
		DrawText("PAUSED", GetScreenWidth() / 2 - 50, 10, 10, RED);

	int crosshairSize = 7;

	DrawRectangle(GetScreenWidth() / 2 - crosshairSize / 2, GetScreenHeight() / 2 - crosshairSize / 2, crosshairSize, crosshairSize, BLACK);

	EndDrawing();
}

void psim::Simulation::setPaused(bool b)
{
	this->bPaused = b;
}

bool psim::Simulation::isPaused()
{
	return this->bPaused;
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
