#pragma once

#include "system.h"
#include "raylib.h"

namespace psim {

	class Simulation
	{

	private:

		System system;
		Camera3D camera;

		int nUpdateCount;
		int nRequiredUpdateCount;
		double simulationTime;
		psim::RigidBody* trackBody;

	public:

		Simulation();

		bool init();

		void update(float fElapsedTime);
		void render();

		bool pause();

		bool run();

	private:
		void processInput(psim::Vector3f& camVel);
		void toggleFullScreen();

	};

}