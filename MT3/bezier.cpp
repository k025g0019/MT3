#include  "bezier.h"

#include "Novice.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Add(Multiply(1.0f - t, v1), Multiply(t, v2));
}

Vector3 Bezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, float t) {
	Vector3 p0 = Lerp(v1, v2, t);
	Vector3 p1 = Lerp(v2, v3, t);
	return Lerp(p0, p1, t);
}

void DrawBezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {
	constexpr int kSegmentCount = 16;
	Vector3 previousPoint = v1;
	for (int32_t index = 1; index <= kSegmentCount; ++index) {
		float t = static_cast<float>(index) / static_cast<float>(kSegmentCount);
		Vector3 point = Bezier(v1, v2, v3, t);
		Vector3 screenPreviousPoint = Transform(Transform(previousPoint, viewProjectionMatrix), viewportMatrix);
		Vector3 screenPoint = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(
			static_cast<int>(screenPreviousPoint.x),
			static_cast<int>(screenPreviousPoint.y),
			static_cast<int>(screenPoint.x),
			static_cast<int>(screenPoint.y),
			color
		);
		previousPoint = point;
	}
}