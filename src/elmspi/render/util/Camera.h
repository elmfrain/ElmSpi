#pragma once
#include "../../maths/Maths.h"

class Camera
{
	public:
		espi::entity entity;

		Camera();
		~Camera();
		espi::matrixf getViewMatrix() const;
		void reset();
		void move(float frontBack, float rightLeft, float upDown);
		void setFov(float fov);
		float getFov() const;
		void setViewportSize(int width, int height);
		int getViewportWidth() const;
		int getViewportHeight() const;
	private:
		float fov = 45.0f;
		float nPlaneDist = 0.01f;
		float fPlaneDist = 500.0f;
		espi::vec2f viewportSize = espi::vec2f(1, 1);
		espi::matrixf getProjectionMatrix() const;
};
