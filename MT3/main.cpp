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
	Segment segment{{-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f}};
	Vector3 point{-1.5f, 0.6f, 0.6f};

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

		Vector3 project = Project(point, Subtract(segment.end, segment.start));
		Vector3 closestPoint = ClosestPoint(point, segment);

		Sphere pointSphere{point, 0.01f};
		Sphere closestPointSphere{closestPoint, 0.01f};
		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

		Vector3 start = Transform(Transform(segment.start, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segment.end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x),
		                 static_cast<int>(end.y), WHITE);


		ImGui::Begin("Debug Window");
		ImGui::DragFloat3("Camera Translate", &cameraTranslate.x, 0.1f);
		ImGui::DragFloat3("Camera Rotate", &cameraRotate.x, 0.01f);
		ImGui::InputFloat3("Priject", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::End();
		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
