#include "math.h"
#include <cmath>
#include <algorithm>
#include "core/mem/mem.h"
#include "cs2/signatures.h"
#include "cs2/helpers/devlog.h"

namespace Math {
	void AngleVectors(const QAngle_t& angles, Vec3& forward, Vec3& right, Vec3& up) {
		float sr, sp, sy, cr, cp, cy;

		sy = sinf(DEG2RAD(angles.yaw));
		cy = cosf(DEG2RAD(angles.yaw));

		sp = sinf(DEG2RAD(angles.pitch));
		cp = cosf(DEG2RAD(angles.pitch));

		sr = sinf(DEG2RAD(angles.roll));
		cr = cosf(DEG2RAD(angles.roll));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = -1 * sr * cp;

		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = cr * cp;
	}

	void NormalizeAngles(QAngle_t& angles) {
		while (angles.pitch > 89.0f) angles.pitch -= 180.f;
		while (angles.pitch < -89.0f) angles.pitch += 180.f;
		while (angles.yaw > 180.f) angles.yaw -= 360.f;
		while (angles.yaw < -180.f) angles.yaw += 360.f;
	}

	void ClampAngles(QAngle_t& angles) {
		if (angles.pitch > 89.0f) angles.pitch = 89.0f;
		if (angles.pitch < -89.0f) angles.pitch = -89.0f;
		if (angles.yaw > 180.f) angles.yaw = 180.f;
		if (angles.yaw < -180.f) angles.yaw = -180.f;
		angles.roll = 0.f;
	}

	bool WorldToScreen(const Vec3& pos, Vec2& screen, const ViewMatrix& matrix, int width, int height) {
		static uintptr_t screenTransformAddr = 0;
		static bool hasSearched = false;
		if (!hasSearched) {
			screenTransformAddr = Mem::PatternScan(SCREENTRANSFORM_PATTERN, CLIENT_DLL);
			hasSearched = true;
			if (screenTransformAddr) {
				DEV_LOG_HEX("[debug] ScreenTransform adresi bulundu: ", screenTransformAddr);
			} else {
				DEV_LOG("[debug] ScreenTransform adresi bulunamadi");
			}
		}

		if (screenTransformAddr) {
			using ScreenTransformFn = bool(__fastcall*)(const Vec3&, Vec3&);
			ScreenTransformFn oScreenTransform = (ScreenTransformFn)screenTransformAddr;

			Vec3 outPos = {};
			bool notClipped = !oScreenTransform(pos, outPos);
			if (notClipped) {
				screen.x = ((outPos.x + 1.0f) * 0.5f) * width;
				screen.y = (float)height - (((outPos.y + 1.0f) * 0.5f) * height);
				return true;
			}
			return false;
		}

		float w = matrix.m[3][0] * pos.x + matrix.m[3][1] * pos.y + matrix.m[3][2] * pos.z + matrix.m[3][3];
		if (w < 0.01f)
			return false;

		float x = matrix.m[0][0] * pos.x + matrix.m[0][1] * pos.y + matrix.m[0][2] * pos.z + matrix.m[0][3];
		float y = matrix.m[1][0] * pos.x + matrix.m[1][1] * pos.y + matrix.m[1][2] * pos.z + matrix.m[1][3];

		float invw = 1.0f / w;
		x *= invw;
		y *= invw;

		float ndcX = (x + 1.0f) * 0.5f;
		float ndcY = (1.0f - y) * 0.5f;

		screen.x = ndcX * width;
		screen.y = ndcY * height;

		return true;
	}
}
