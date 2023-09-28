#pragma once

#include "system.h"
#include "raylib.h"

namespace psim {

	class Simulation
	{

	private:

		System system;
		Camera3D camera;

		bool bPaused;
		int nUpdateCount;
		int nRequiredUpdateCount;
		double simulationTime;
		psim::RigidBody* trackBody;
		psim::RigidBody* infoBody;
		psim::RigidBody* springBody;


	public:

		Simulation();

		bool init();

		void update(float fElapsedTime);
		void render();

		void setPaused(bool b);
		bool isPaused();

		bool run();

	private:
		void processInput(psim::Vector3f& camVel);
		void toggleFullScreen();

	};

}