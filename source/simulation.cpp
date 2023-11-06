#include <iostream>
#include <iomanip>
#include <sstream>
#include "simulation.h"
#include "utils.h"
#include "sphere.h"
#include "cuboid.h"
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

psim::System psim::Simulation::system;

psim::Simulation::Simulation()
{
	paused = false;
	trackBody = nullptr;
	springBody = nullptr;
	infoBody = nullptr;
	simulationTime = 0;
	trackBodyDistance = 0; 
	trackBodyOffset = Vector3f::ZERO;
	camera = { 0 };
	nUpdateCount = 0;
	nRequiredUpdateCount = 0;
	system.~System();
	system = std::move(System());
}

psim::Simulation::~Simulation()
{
}

bool psim::Simulation::init()
{

	std::vector<RigidBody*> test;


	// init camera
	camera.position = psim::Vector3f{ 0.0f, 10.0f, 25.0f };
	camera.target = psim::Vector3f{ 0.0f, 4.0f, 0.0f };
	camera.up = psim::Vector3f{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	// add objects
	static const int object_count = 4;
	for (int k = 0; k < 3; k++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < object_count; i++)
			{
				float r = 1;
				psim::RigidBody* body = new psim::RigidBody(
					Vector3f{j * 2 * r, i * 2 * r + r, k * 2 * r},
					new psim::Sphere(r)
				);
				// body->setRestitution(0);
				system.addRigidBody(body);
			}
		}
	}

	psim::RigidBody* a = new psim::RigidBody(Vector3f{ -10, 2, 0}, new psim::Sphere(1.0f));
	psim::RigidBody* b = new psim::RigidBody(Vector3f{ -10, 20, 0 }, new psim::Sphere(1.0f));
	psim::RigidBody* c = new psim::RigidBody(Vector3f{ 0, 5, 0 }, new psim::Cuboid());

	system.addRigidBody(a);
	system.addRigidBody(b);
	system.addRigidBody(c);

	a->getVel() = Vector3f{ 1, 0, 0 };

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

	//std::cout << Vector3f{camera.position}.toString() << std::endl;

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

	SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

	timestamp timeInput = Clock::now();

	if (!paused)
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

	if (IsKeyPressed(KEY_P))
	{
		this->paused = !paused;
		
		//if (paused)
		//{
		//	ShowCursor();
		//}
		//else
		//{
		//	HideCursor();
		//}

	}

	if (IsKeyPressed(KEY_F1))
	{
		takeScreenshot();
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
			ray.position = camera.position;
			Vector3f contactPoint;
			trackBody = system.raycastSelect(ray, contactPoint);
			if (trackBody)
			{
				trackBodyDistance = Vector3f{ camera.position - contactPoint }.mag();
				trackBodyOffset = trackBody->getPos() - contactPoint;
				trackBody->setColor(GREEN);
			}
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
			
			ray.direction = Vector3Normalize(camera.target - camera.position);
			ray.position = camera.position;
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

	if (trackBody)
	{
		trackBodyDistance += GetMouseWheelMove();
		if (trackBodyDistance < 1.0f)
		{
			trackBodyDistance = 1.0f;
		}
	}
}

psim::Vector3f psim::Simulation::getCameraDirection(const Camera& cam) const
{
	return Vector3f{ cam.target - cam.position }.normalize();
}

float psim::Simulation::getDistanceCameraToBody(const Camera& cam, psim::RigidBody& body)
{
	return (body.getPos() - Vector3f{cam.position}).mag();
}

void psim::Simulation::update(float fElapsedTime)
{

	system.clearForces();
	system.checkCollision();


	if (trackBody)
	{
		trackBody->getPos() = Vector3f{ camera.position } + ( getCameraDirection(camera) * trackBodyDistance ) + trackBodyOffset;
		trackBody->getVel() = Vector3f::ZERO;
	}

	if (springBody)
	{
		Vector3f diff = (Vector3f{ camera.target } - springBody->getPos());
		Vector3f dir = diff.normalize();

		float kx = 10;

		Vector3f springForce = diff * kx * springBody->getMass();
		springBody->applyForce(springForce);

	}

	system.applyGravity();

	// TODO: delete this, it only stops balls from fallung through the floor
	// system.step(fElapsedTime);
	
	
	StateVector in = system.getStateVector();
	StateVector out;
	float time = static_cast<float>(GetTime());
	solver.step(in, out, time, fElapsedTime, system_dydt);
	system.update(out);

}

void psim::Simulation::render()
{
	BeginDrawing();

	ClearBackground(WHITE);

	BeginMode3D(camera);

		for (psim::RigidBody* body : system.getObjects())
		{
			body->draw();
			BoundingBox box = body->getAABB();
			DrawBoundingBox(box, GREEN);
		}

		if (springBody)
		{
			DrawLine3D(camera.target, springBody->getPos(), BLACK);
		}

		if (trackBody)
		{
			Vector3f center = trackBody->getPos();
			center.y = 0;
			DrawCircle3D(center, 0.25f, Vector3f{ 1, 0, 0 }, 90, MAGENTA);
			DrawLine3D(trackBody->getPos(), center, LIGHTGRAY);
		}

		rotation.x += 0.01;
		rotation.y += 0.02;
		model1.transform = MatrixRotateXYZ(rotation);

		//DrawModel(model1, psim::Vector3f{1, 1, 1}, 3, GREEN);
	    //DrawModelWires(model1, psim::Vector3f{1, 1, 1}, 3, BLACK);

		DrawGrid(100, 1.0f);

	EndMode3D();

	DrawFPS(10, 10);

	char buffer[400 + 1];

	DrawText(TextFormat("Camera\nx = %.2f y = %.2f z = %.2f\nTarget\nx = %.2f y = %.2f z = %.2f",
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z), 10, 30, 5, BLACK);

	//DrawText(TextFormat("Update Count: %03d / %03d", nUpdateCount, nRequiredUpdateCount), GetScreenWidth() - 125, 10, 5, RED);
	DrawText(TextFormat("Simulation Time: %3.2f", simulationTime), GetScreenWidth() - 125, 30, 5, BLACK);
	DrawText(TextFormat("Energy: %3.2f", system.getTotalEnergy()), GetScreenWidth() - 125, 50, 5, PURPLE);

	if (infoBody != nullptr)
	{

		Vector3f& pos = infoBody->getPos();
		Vector3f& vel = infoBody->getVel();
		Vector3f& acc = infoBody->getAcc();
		float mass = infoBody->getMass();

		const char* text = TextFormat("RigidBody\npos { x: %2.2f y: %2.2f z: %2.2f }\nvel { x: %2.2f y: %2.2f z: %2.2f }\nacc { x: %2.2f y: %2.2f z: %2.2f }\nmass: %2.2f\n",
			pos.x, pos.y, pos.z,
			vel.x, vel.y, vel.z,
			acc.x, acc.y, acc.z,
			mass
		);
		DrawText(text, GetScreenWidth() - 200, 50, 3, BLACK);
	}

	if (paused)
		DrawText("PAUSED", GetScreenWidth() / 2 - 50, 10, 10, RED);

	int crosshairSize = 7;

	DrawRectangle(GetScreenWidth() / 2 - crosshairSize / 2, GetScreenHeight() / 2 - crosshairSize / 2, crosshairSize, crosshairSize, BLACK);

	EndDrawing();
}

void psim::Simulation::setPaused(bool b)
{
	this->paused = b;
}

bool psim::Simulation::isPaused()
{
	return this->paused;
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

void psim::Simulation::takeScreenshot()
{
	std::string folderPath = GetWorkingDirectory();
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
	auto str = oss.str();
	std::string filename = "screenshot_" + str;
	
	std::string absolutePath = folderPath + "\\" + filename + ".png";
	std::cout << "Screenshot saved to " << absolutePath << std::endl;
	
	TakeScreenshot(filename.c_str());
}
