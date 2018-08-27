#pragma once
namespace random {
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float generate_noise(float x, float y, float z);
}