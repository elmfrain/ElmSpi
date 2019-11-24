#include "Camera.h"

using namespace espi;

Camera::Camera()
{
}

Camera::~Camera()
{
}

matrixf Camera::getViewMatrix() const
{
	float translationFactor[] =
	{
		1, 0, 0, -entity.pos.x,
		0, 1, 0, -entity.pos.y,
		0, 0, 1,  entity.pos.z,
		0, 0, 0,             1
	}; matrixf translation(translationFactor, 4, 4);

	float rotPitch = -toRadians(entity.rotPitch);
	float rotYaw = toRadians(entity.rotYaw);
	float rotRoll = toRadians(entity.rotRoll);

	float cosZ = cosf(rotRoll);
	float sinZ = sinf(rotRoll);

	float cosY = cosf(rotYaw);
	float sinY = sinf(rotYaw);

	float cosX = cosf(rotPitch);
	float sinX = sinf(rotPitch);

	float rollFactor[] =
	{
		cosZ,-sinZ, 0, 0,
		sinZ, cosZ, 0, 0,
		   0,    0, 1, 0,
		   0,    0, 0, 1
	}; matrixf roll(rollFactor, 4, 4);
	float yawFactor[] =
	{
		cosY, 0, sinY, 0,
		   0, 1,    0, 0,
	   -sinY, 0, cosY, 0,
		   0, 0,    0, 1
	}; matrixf yaw(yawFactor, 4, 4);
	float pitchFactor[] =
	{
		1,    0,    0, 0,
		0, cosX,-sinX, 0,
		0, sinX, cosX, 0,
		0,    0,    0, 1
	}; matrixf pitch(pitchFactor, 4, 4);
	return getProjectionMatrix() * roll * pitch * yaw * translation;
	//return matrixf(getProjectionMatrix().array, 4, 4);
}

void Camera::reset()
{
	entity.reset();
	fov = 45.0f;
	nPlaneDist = 0.01f;
	fPlaneDist = 500.0f;
}

void Camera::move(float frontBack, float rightLeft, float upDown)
{
	entity.pos.x += sinf(toRadians(entity.rotYaw)) * frontBack;
	entity.pos.z += cosf(toRadians(entity.rotYaw)) * frontBack;

	entity.pos.x += cosf(toRadians(entity.rotYaw)) * rightLeft;
	entity.pos.z -= sinf(toRadians(entity.rotYaw)) * rightLeft;

	entity.pos.y += upDown;
}

void Camera::setFov(float fov)
{
	this->fov = fov;
}

float Camera::getFov() const
{
	return this->fov;
}

void Camera::setViewportSize(int width, int height)
{
	viewportSize.x = width;
	viewportSize.y = height;
}

int Camera::getViewportWidth() const
{
	return viewportSize.x;
}

int Camera::getViewportHeight() const
{
	return viewportSize.y;
}

matrixf Camera::getProjectionMatrix() const
{
	float nPlaneHeight = tanf(toRadians(fov / 2.0f)) * nPlaneDist;
	float nPlaneWidth = nPlaneHeight * (viewportSize.x / viewportSize.y);

	float projectionFactor[] =
	{
		nPlaneDist / nPlaneWidth,                         0,                                                      0,                                                             0,
		                       0, nPlaneDist / nPlaneHeight,                                                      0,                                                             0,
							   0,                         0, -(fPlaneDist + nPlaneDist) / (fPlaneDist - nPlaneDist), (-2.0f * fPlaneDist * nPlaneDist) / (fPlaneDist - nPlaneDist),
							   0,                         0,                                                     -1,                                                             0
	};

	return matrixf(projectionFactor, 4, 4);
}
