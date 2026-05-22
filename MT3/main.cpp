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

	Segment segment{
		.origin{0.0f, 0.0f, 0.0f},
		.diff{0.5f, 0.0f, 0.0f},
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


		DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);

		if (AABBIntersectsSegment(aabb1, segment)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}

		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("aabb.min", &aabb1.min.x, 0.1f);
		ImGui::DragFloat3("aabb.max", &aabb1.max.x, 0.1f);
		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.1f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.1f);


		ImGui::End();

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
