#include "sdk.h"
#include "Math.h"
#include <random>

float Math::RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

float Math::RandomFloat2(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	return ((RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"))(min, max);
}
inline Vector Math::CalcAnglexd(const Vector& src, const Vector& dst)
{
	Vector ret;
	VectorAngles(dst - src, ret);
	return ret;
}
void Math::CalcAngle(Vector src, Vector dst, Vector& angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = atan(delta.y / delta.x) * 57.295779513082f;
	angles.x = atan(delta.z / hyp) * 57.295779513082f;
	if (delta.x >= 0.0)
		angles.y += 180.0f;
	angles.z = 0;
}

void Math::AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	auto SinCos = [](float flRadians, float* pflSine, float* pflCosine) {
		__asm
		{
			fld	DWORD PTR[flRadians]
			fsincos
			mov edx, DWORD PTR[pflCosine]
			mov eax, DWORD PTR[pflSine]
			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
		}
	};
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

inline float Math::GetYawDelta(float yaw1, float yaw2)
{
	return fabs(NormalizeYaw(yaw1 - yaw2));
}
inline float Math::RandomNumber(float min, float max)
{
	static std::random_device rand_device;
	static std::mt19937 rand_gen(rand_device());

	std::uniform_real_distribution<float> rand_distributer(min, max);
	return rand_distributer(rand_gen);
}
float Math::NormalizeYaw(float value)
{
	while (value > 180)
		value -= 360.f;

	while (value < -180)
		value += 360.f;
	return value;
}

void Math::VectorAngles(const Vector& vecForward, Vector& vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f) {
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else {
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void Math::AngleVectors(const Vector angles, Vector& forward, Vector& right, Vector& up)
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[0] * (M_PI / 180.f);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[1] * (M_PI / 180.f);
	sy = sin(angle);
	cy = cos(angle);

	forward[0] = cp * cy;
	forward[1] = cp * sy;
	forward[2] = -sp;

	static float sr, cr;

	angle = angles[2] * (M_PI / 180.f);
	sr = sin(angle);
	cr = cos(angle);

	right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
	right[1] = -1 * sr * sp * sy + -1 * cr * cy;
	right[2] = -1 * sr * cp;

	up[0] = cr * sp * cy + -sr * -sy;
	up[1] = cr * sp * sy + -sr * cy;
	up[2] = cr * cp;
}

void Math::NormalizeVector(Vector& vecIn) 
{
	for (int axis = 0; axis < 3; ++axis) {
		while (vecIn[axis] > 180.f)
			vecIn[axis] -= 360.f;

		while (vecIn[axis] < -180.f)
			vecIn[axis] += 360.f;
	}

	vecIn[2] = 0.f;
}
inline float Math::NormalizePitch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}
inline Vector Math::NormalizeAngle(Vector angle)
{
	angle.x = NormalizePitch(angle.x);
	angle.y = NormalizeYaw(angle.y);

	if ((angle.z > 50) || (angle.z < 50))
		angle.z = 0;

	return angle;
}

void Math::AngleVectors(const QAngle& angles, Vector* forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void SinCosBitch(float a, float* s, float* c)
{
	*s = sin(a);
	*c = cos(a);
}

void Math::AngleVectors(const QAngle& angles, Vector& forward)
{
	float sp, sy, cp, cy;

	SinCosBitch(DEG2RAD(angles[1]), &sy, &cy);
	SinCosBitch(DEG2RAD(angles[0]), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void Math::ClampAngles(QAngle& angles)
{
	if (angles.x > 89.f)
		angles.x = 89.f;
	if (angles.x < -89.f)
		angles.x = -89.f;
	if (angles.y > 180.f)
		angles.y = 180.f;
	if (angles.y < -180.f)
		angles.y = -180.f;

	angles.z = 0.f;
}






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































