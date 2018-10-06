#pragma once
namespace Math
{
	extern float NormalizeYaw(float value);
	extern void VectorAngles(const Vector&vecForward, Vector&vecAngles);
	extern void AngleVectors(const Vector angles, Vector& forward, Vector& right, Vector& up);
	extern void VectorMA(const Vector & start, float scale, const Vector & direction, Vector & dest);
	extern void NormalizeVector(Vector & vecIn);
	extern inline float NormalizePitch(float pitch);
	extern inline Vector NormalizeAngle(Vector angle);
	extern void AngleVectors(const QAngle &angles, Vector* forward);
	extern void AngleVectors(const QAngle & angles, Vector & forward);
	extern void ClampAngles(QAngle& angles);
	extern float RandomFloat(float min, float max);
	extern float RandomFloat2(float min, float max);
	extern inline Vector CalcAnglexd(const Vector & src, const Vector & dst);
	extern void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	extern inline float GetYawDelta(float yaw1, float yaw2);
	extern inline float RandomNumber(float min, float max);
	extern void CalcAngle(Vector src, Vector dst, Vector& angles);
}

extern bool enabledtp;