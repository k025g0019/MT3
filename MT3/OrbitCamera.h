#pragma once

#include "Vector.h"

struct OrbitCamera {
	Vector3 target;
	float distance;
	float yaw;
	float pitch;
	int lastMouseX;
	int lastMouseY;
	bool isDragging;
};

void InitializeOrbitCamera(OrbitCamera& camera, const Vector3& target, const Vector3& position);
void UpdateOrbitCamera(OrbitCamera& camera, bool enableMouseInput);
Vector3 GetOrbitCameraPosition(const OrbitCamera& camera);
Vector3 GetOrbitCameraRotation(const OrbitCamera& camera);
