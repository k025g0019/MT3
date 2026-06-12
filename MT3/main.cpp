#include <Novice.h>

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <algorithm>
#include <cmath>

#include "OrbitCamera.h"
#include "Vector&Matrix.h"
#include "bezier.h"

constexpr char kWindowTitle[] = "LE1B_26";
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});

	Ball ball{};
	ball.position = {0.8f, 1.2f, 0.3f};
	ball.color = BLUE;
	ball.mass = 2.0f;
	ball.velocity = {0.0f, 0.0f, 0.0f};
	ball.radius = 0.05f;
	ball.acceleration = {0.0f, -9.8f, 0.0f};

	Plane plane{};
	plane.distance = 0.0f;
	plane.normal = Normalize({-0.2f, 0.9f, -0.3f});

	constexpr float deltaTime = 1.0f / 60.0f;
	constexpr float kRestitution = 0.5f;

	Sphere sphere{};

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

#ifdef USE_IMGUI
		const bool canControlCamera = !ImGui::GetIO().WantCaptureMouse;
#else
		const bool canControlCamera = true;
#endif

		UpdateOrbitCamera(orbitCamera, canControlCamera);
		Vector3 cameraTranslate = GetOrbitCameraPosition(orbitCamera);
		Vector3 cameraRotate = GetOrbitCameraRotation(orbitCamera);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		Vector3 prePosition = ball.position;

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;

		Capsule capsule{};
		capsule.segment.origin = prePosition;
		capsule.segment.diff = ball.position - prePosition;
		capsule.radius = ball.radius;

		float preDistance = Dot(prePosition, plane.normal) - plane.distance;
		float currentDistance = Dot(ball.position, plane.normal) - plane.distance;

		bool isCapsuleHit =
			(preDistance > ball.radius && currentDistance <= ball.radius) ||
			(preDistance < -ball.radius && currentDistance >= -ball.radius);

		if (isCapsuleHit) {
			Vector3 pushDirection = currentDistance >= 0.0f ? plane.normal : -1.0f * plane.normal;
			float penetration = ball.radius - std::abs(currentDistance);
			ball.position += pushDirection * penetration;

			Vector3 normalVelocity = Project(ball.velocity, pushDirection);
			Vector3 tangentVelocity = ball.velocity - normalVelocity;
			Vector3 reflectedNormal = Reflect(normalVelocity, pushDirection);
			ball.velocity = tangentVelocity + kRestitution * reflectedNormal;
		}

		sphere = Sphere{ball.position, ball.radius};

		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, ball.color);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, RED);

#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");
		if (ImGui::Button("Reset Ball")) {
			ball.position = {0.8f, 1.2f, 0.3f};
			ball.color = BLUE;
			ball.mass = 2.0f;
			ball.velocity = {0.0f, 0.0f, 0.0f};
			ball.radius = 0.05f;
			ball.acceleration = {0.0f, -9.8f, 0.0f};
		}
		ImGui::End();
#endif

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
