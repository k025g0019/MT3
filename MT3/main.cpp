#include <Novice.h>

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <algorithm>

#include "OrbitCamera.h"
#include "Vector&Matrix.h"

constexpr char kWindowTitle[] = "LE1B_26";
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{0.0f, 0.0f, 0.0f};

	OBB obb{
		.center{-1.0f, 0.0f, 0.0f},
		.orientations{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
		},
		.size{0.5f, 0.5f, 0.5f},
	};

	Sphere sphere{
		.center{0.0f, 0.0f, 0.0f},
		.radius{0.5f},
	};
	uint32_t obbColor = WHITE;
	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});

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
		obb.size.x = (std::max)(obb.size.x, 0.01f);
		obb.size.y = (std::max)(obb.size.y, 0.01f);
		obb.size.z = (std::max)(obb.size.z, 0.01f);
		sphere.radius = (std::max)(sphere.radius, 0.01f);

		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		Matrix4x4 rotateMatrix = Multiply(
			MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
		obb.orientations[0] = {rotateMatrix.matrix[0][0], rotateMatrix.matrix[0][1], rotateMatrix.matrix[0][2]};
		obb.orientations[1] = {rotateMatrix.matrix[1][0], rotateMatrix.matrix[1][1], rotateMatrix.matrix[1][2]};
		obb.orientations[2] = {rotateMatrix.matrix[2][0], rotateMatrix.matrix[2][1], rotateMatrix.matrix[2][2]};


		if (OBBToSphereIsCollision(obb, sphere)) {
			obbColor = RED;
		}
		else {
			obbColor = WHITE;
		}
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawOBB(obb, viewProjectionMatrix, viewportMatrix, obbColor);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, WHITE);

#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("obb.center", &obb.center.x, 0.1f);
		ImGui::DragFloat3("obb.orientations[0]", &obb.orientations[0].x, 0.1f);
		ImGui::DragFloat3("obb.orientations[1]", &obb.orientations[1].x, 0.1f);
		ImGui::DragFloat3("obb.orientations[2]", &obb.orientations[2].x, 0.1f);
		ImGui::DragFloat3("obb.size", &obb.size.x, 0.1f, 0.01f);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.1f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.1f, 0.01f);

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
