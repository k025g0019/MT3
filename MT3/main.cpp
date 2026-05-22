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

	Vector3 rotate1{0.0f, 0.0f, 0.0f};
	Vector3 rotate2{-0.05f, -2.49f, 0.15f};

	OBB obb1{
		.center{0.0f, 0.0f, 0.0f},
		.orientations{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
		},
		.size{0.83f, 0.26f, 0.24f},
	};

	OBB obb2{
		.center{0.9f, 0.66f, 0.78f},
		.orientations{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
		},
		.size{0.5f, 0.37f, 0.5f},
	};

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
		obb1.size.x = (std::max)(obb1.size.x, 0.01f);
		obb1.size.y = (std::max)(obb1.size.y, 0.01f);
		obb1.size.z = (std::max)(obb1.size.z, 0.01f);
		obb2.size.x = (std::max)(obb2.size.x, 0.01f);
		obb2.size.y = (std::max)(obb2.size.y, 0.01f);
		obb2.size.z = (std::max)(obb2.size.z, 0.01f);


		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		Matrix4x4 rotateMatrix1 = Multiply(
			MakeRotateXMatrix(rotate1.x), Multiply(MakeRotateYMatrix(rotate1.y), MakeRotateZMatrix(rotate1.z)));
		Matrix4x4 rotateMatrix2 = Multiply(
			MakeRotateXMatrix(rotate2.x), Multiply(MakeRotateYMatrix(rotate2.y), MakeRotateZMatrix(rotate2.z)));
		obb1.orientations[0] = {rotateMatrix1.matrix[0][0], rotateMatrix1.matrix[0][1], rotateMatrix1.matrix[0][2]};
		obb1.orientations[1] = {rotateMatrix1.matrix[1][0], rotateMatrix1.matrix[1][1], rotateMatrix1.matrix[1][2]};
		obb1.orientations[2] = {rotateMatrix1.matrix[2][0], rotateMatrix1.matrix[2][1], rotateMatrix1.matrix[2][2]};
		obb2.orientations[0] = {rotateMatrix2.matrix[0][0], rotateMatrix2.matrix[0][1], rotateMatrix2.matrix[0][2]};
		obb2.orientations[1] = {rotateMatrix2.matrix[1][0], rotateMatrix2.matrix[1][1], rotateMatrix2.matrix[1][2]};
		obb2.orientations[2] = {rotateMatrix2.matrix[2][0], rotateMatrix2.matrix[2][1], rotateMatrix2.matrix[2][2]};

		bool isCollision = OBBToOBBIsCollision(obb1, obb2);
		uint32_t obb1Color = isCollision ? RED : WHITE;
		uint32_t obb2Color = isCollision ? RED : WHITE;
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, obb1Color);
		DrawOBB(obb2, viewProjectionMatrix, viewportMatrix, obb2Color);

#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");
		ImGui::Text("collision: %s", isCollision ? "true" : "false");
		ImGui::DragFloat3("rotate1", &rotate1.x, 0.01f);
		ImGui::DragFloat3("rotate2", &rotate2.x, 0.01f);
		ImGui::DragFloat3("obb1.center", &obb1.center.x, 0.1f);
		ImGui::DragFloat3("obb1.size", &obb1.size.x, 0.1f, 0.01f);
		ImGui::DragFloat3("obb2.center", &obb2.center.x, 0.1f);
		ImGui::DragFloat3("obb2.size", &obb2.size.x, 0.1f, 0.01f);

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
