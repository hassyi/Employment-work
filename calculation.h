#pragma once
#include "main.h"

inline float Dot(const XMFLOAT3 vec1, const XMFLOAT3 vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline float Length(const XMFLOAT3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline XMFLOAT3 Normalize(const XMFLOAT3 vec)
{
	float len = Length(vec);
	if (len != 0.0f) {
		return XMFLOAT3(vec.x / len, vec.y / len, vec.z / len);
	}
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

inline XMFLOAT3 Add(const XMFLOAT3 vec1, const XMFLOAT3 vec2) {
	return XMFLOAT3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

inline XMFLOAT3 Sub(const XMFLOAT3 vec1, const XMFLOAT3 vec2) {
	return XMFLOAT3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

inline XMFLOAT3 Mul(const XMFLOAT3 vec1, const XMFLOAT3 vec2) {
	return XMFLOAT3(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
}

inline XMFLOAT3 MulFloat(const XMFLOAT3 vec1, const float a) {
	return XMFLOAT3(vec1.x * a, vec1.y * a, vec1.z * a);
}

inline float LengthSquared(const XMFLOAT3 vec){
	return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
}