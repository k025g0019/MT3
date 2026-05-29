#include <Novice.h>

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <algorithm>

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

	//カメラの初期化
	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});

	Spring spring{};
	spring.anchor = {0.0f, 0.0f, 0.0f};
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = {1.2f, 0.0f, 0.0f};
	ball.velocity = {0.0f, 0.0f, 0.0f};
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;


	float deltaTime = 1.0f / 60.0f;
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		//========================================================
		// 必須
		//============================================================
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

		//========================================================
		// ここにコードを追加していく
		//=====================================================

		Vector3 diff = ball.position - spring.anchor;
		float length = Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Normalize(diff);
			Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
			Vector3 displacement = length * (ball.position - restPosition);
			Vector3 restoringForce = -spring.stiffness * displacement;
			Vector3 force = restoringForce;
			ball.acceleration = force * (1.0f / ball.mass);
			//減衰抵抗を計算する
			Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
			ball.acceleration += dampingForce * (1.0f / ball.mass);
		}

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;


		Sphere ballSphere{};
		Segment segmentBall{};
		ballSphere.center = ball.position;
		ballSphere.radius = ball.radius;
		segmentBall = {
			.origin{spring.anchor},
			.diff{ball.position - spring.anchor}
		};

		DrawSphere(ballSphere, viewProjectionMatrix, viewportMatrix, ball.color);
		DrawSegment(segmentBall, viewProjectionMatrix, viewportMatrix, RED);
#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");

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
