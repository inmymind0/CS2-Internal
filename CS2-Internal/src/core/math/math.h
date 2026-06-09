#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define DEG2RAD(x) ((float)(x) * (float)(M_PI / 180.f))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI))

struct Vec2 {
	float x, y;

	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float _x, float _y) : x(_x), y(_y) {}
};

struct Vec3 {
	float x, y, z;
	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	inline float Dot(const Vec3& b) const {
		return x * b.x + y * b.y + z * b.z;
	}

	inline Vec3 operator+(const Vec3& b) const {
		return Vec3(x + b.x, y + b.y, z + b.z);
	}

	inline void Normalize() {
		float len = sqrtf(x * x + y * y + z * z);
		if (len != 0.f) {
			x /= len;
			y /= len;
			z /= len;
		}
	}
};

struct QAngle_t {
	float pitch, yaw, roll;

	QAngle_t() : pitch(0.0f), yaw(0.0f), roll(0.0f) {}
	QAngle_t(float _pitch, float _yaw, float _roll) : pitch(_pitch), yaw(_yaw), roll(_roll) {}
};

struct ViewMatrix {
	float m[4][4];
};

namespace Math {
	void AngleVectors(const QAngle_t& angles, Vec3& forward, Vec3& right, Vec3& up);
	void NormalizeAngles(QAngle_t& angles);
	void ClampAngles(QAngle_t& angles);
	bool WorldToScreen(const Vec3& pos, Vec2& screen, const ViewMatrix& matrix, int width, int height);
}