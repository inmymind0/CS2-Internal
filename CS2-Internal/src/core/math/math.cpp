#include "math.h"
#include <cmath>
#include <algorithm>

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
}
