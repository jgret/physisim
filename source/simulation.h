#pragma once

#include "system.h"
#include "raylib.h"

namespace psim {

	class Simulation
	{

	private:

		System system;
		Camera3D camera;

		bool paused;
		int nUpdateCount;
		int nRequiredUpdateCount;
		double simulationTime;
		float trackBodyDistance;			// distance to hold body at when dragging around
		psim::Vector3f trackBodyOffset;
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
		psim::Vector3f getCameraDirection(const Camera& cam) const;
		float getDistanceCameraToBody(const Camera& cam, psim::RigidBody& body);
		void toggleFullScreen();
		void takeScreenshot();

	};

}