#include <Novice.h>

#ifdef _DEBUG
#include <imgui.h>
#endif

#include "Vector&Matrix.h"

constexpr char kWindowTitle[] = "LE1B_26";
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	Sphere sphere{{0.0f, 0.0f, 0.0f}, 1.0f};
	Vector3 cameraTranslate{0.0f, 1.9f, -6.49f};
	Vector3 cameraRotate{0.26f, 0.0f, 0.0f};
	Sphere sphere2{{1.0f, 1.0f, 0.0f}, 1.0f};

	Plane plane{{0.0f, 1.0f, 0.0f}, 0.0f};
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		DrawGrid(viewProjectionMatrix, viewportMatrix);


		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);


		if (PlaneIsCollision(sphere, plane)) {
			DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, WHITE);
		}

		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("Sphere.Center", &sphere.center.x, 0.1f);
		ImGui::DragFloat("Sphere.Radius", &sphere.radius, 0.1f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.1f);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.1f);

		ImGui::End();
		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
