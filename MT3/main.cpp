#include <Novice.h>

#ifdef _DEBUG
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


	AABB aabb1{
		.min{-0.5f, -0.5f, -0.5f},
		.max{0.0f, 0.0f, 0.0f},
	};

	AABB aabb2{
		.min{0.2f, 0.2f, 0.2f},
		.max{1.0f, 1.0f, 1.0f},
	};

	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

#ifdef _DEBUG
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


		DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, WHITE);

		if (AABBToAABBIsCollision(aabb1, aabb2)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}


		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.1f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.1f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.1f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.1f);
		ImGui::Text("Mouse: Left drag rotate / Wheel zoom");
		ImGui::DragFloat3("camera.target", &orbitCamera.target.x, 0.1f);
		ImGui::DragFloat("camera.distance", &orbitCamera.distance, 0.1f, 2.0f, 20.0f);

		ImGui::End();

		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x - 0.1f);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y - 0.1f);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z - 0.1f);
		aabb1.max.x = (std::max)(aabb1.max.x, aabb1.min.x + 0.1f);
		aabb1.max.y = (std::max)(aabb1.max.y, aabb1.min.y + 0.1f);
		aabb1.max.z = (std::max)(aabb1.max.z, aabb1.min.z + 0.1f);
		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x - 0.1f);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y - 0.1f);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z - 0.1f);
		aabb2.max.x = (std::max)(aabb2.max.x, aabb2.min.x + 0.1f);
		aabb2.max.y = (std::max)(aabb2.max.y, aabb2.min.y + 0.1f);
		aabb2.max.z = (std::max)(aabb2.max.z, aabb2.min.z + 0.1f);
		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
