#ifndef VECTOR3__H
#define VECTOR3__H

struct Vector3
{
	static void UnitTest();

	float X;
	float Y;
	float Z;

	static Vector3 Zero;
	static Vector3 One;
	static Vector3 UnitX;
	static Vector3 UnitY;
	static Vector3 UnitZ;
	Vector3(float x, float y, float z);
	Vector3(float value);
	Vector3();


	float Length();
	float LengthSquared();

	static float Distance(const Vector3& value1, const Vector3& value2);
	static float DistanceSquared(const Vector3& value1, const Vector3& value2);
	static float Dot(const Vector3& value1, const Vector3& value2);

	void Normalize();
	static Vector3 Normalize(const Vector3& value);

	static Vector3 Reflect(const Vector3& vector, const Vector3& normal);
	static Vector3 Min(const Vector3& value1, const Vector3& value2);
	static Vector3 Max(const Vector3& value1, const Vector3& value2);
	static Vector3 Clamp(const Vector3& value1, const Vector3& min, const Vector3& max);
	static Vector3 Lerp(const Vector3& value1, const Vector3& value2, float amount);
	static Vector3 Negate(const Vector3& value);
};

bool operator ==(const Vector3& value1, const Vector3& value2);
bool operator !=(const Vector3& value1, const Vector3& value2);

Vector3 operator -(const Vector3& value);
Vector3 operator -(const Vector3& value1, const Vector3& value2);
Vector3 operator +(const Vector3& value1, const Vector3& value2);
Vector3 operator /(const Vector3& value, float divider);
Vector3 operator *(const Vector3& value, float scaleFactor);
Vector3 operator *(float scaleFactor, const Vector3& value);


#endif //VECTOR3__H