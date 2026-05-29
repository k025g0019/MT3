#include "OrbitCamera.h"

#include <Novice.h>

#include <algorithm>
#include <cmath>

namespace {

constexpr float kMinDistance = 2.0f;
constexpr float kMaxDistance = 20.0f;
constexpr float kRotateSensitivity = 0.01f;
constexpr float kZoomSensitivity = 0.0025f;
constexpr float kPitchLimit = 1.45f;

}

void InitializeOrbitCamera(OrbitCamera& camera, const Vector3& target, const Vector3& position) {
	camera.target = target;

	Vector3 offset = Subtract(position, target);
	float distance = Length(offset);
	if (distance <= 0.0001f) {
		offset = {0.0f, 0.0f, -kMinDistance};
		distance = kMinDistance;
	}

	camera.distance = std::clamp(distance, kMinDistance, kMaxDistance);
	camera.pitch = std::asin(std::clamp(offset.y / camera.distance, -1.0f, 1.0f));
	camera.yaw = std::atan2(-offset.x, -offset.z);
	Novice::GetMousePosition(&camera.lastMouseX, &camera.lastMouseY);
	camera.isDragging = false;
}

void UpdateOrbitCamera(OrbitCamera& camera, bool enableMouseInput) {
	int mouseX = 0;
	int mouseY = 0;
	Novice::GetMousePosition(&mouseX, &mouseY);

	if (enableMouseInput) {
		camera.distance = std::clamp(
			camera.distance - static_cast<float>(Novice::GetWheel()) * kZoomSensitivity, kMinDistance, kMaxDistance);

		if (Novice::IsPressMouse(0) != 0) {
			if (camera.isDragging) {
				int deltaX = mouseX - camera.lastMouseX;
				int deltaY = mouseY - camera.lastMouseY;
				camera.yaw += static_cast<float>(deltaX) * kRotateSensitivity;
				camera.pitch = std::clamp(
					camera.pitch - static_cast<float>(deltaY) * kRotateSensitivity, -kPitchLimit, kPitchLimit);
			}
			camera.isDragging = true;
		} else {
			camera.isDragging = false;
		}
	} else {
		camera.isDragging = false;
	}

	camera.lastMouseX = mouseX;
	camera.lastMouseY = mouseY;
}

Vector3 GetOrbitCameraPosition(const OrbitCamera& camera) {
	float cosPitch = std::cos(camera.pitch);
	Vector3 forward{
		std::sin(camera.yaw) * cosPitch,
		-std::sin(camera.pitch),
		std::cos(camera.yaw) * cosPitch,
	};
	return Subtract(camera.target, Multiply(camera.distance, forward));
}

Vector3 GetOrbitCameraRotation(const OrbitCamera& camera) { return {camera.pitch, camera.yaw, 0.0f}; }
