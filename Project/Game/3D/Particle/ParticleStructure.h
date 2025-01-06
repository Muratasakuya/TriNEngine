#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector4.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/Adapter/Easing.h>

// c++
#include <cstdint>
#include <optional>

//============================================================================*/
//	ParticleStructure
//============================================================================*/

struct ParticleTransform {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct ParticleValue {

	Vector3 min;
	Vector3 max;
	Vector3 uniform;
};

enum class RandomParticleColor {

	RED,    //* 赤の色域
	GREEN,  //* 緑の色域
	BLUE,   //* 青の色域
	PURPLE, //* 紫の色域
	GRAY,   //* 灰色の色域

	//* Add
	DARKBLUE, //* 濃い青
};

struct ParticlePhysics {

	bool reflectEnable;                      // 反射するかしないか
	Vector3 reflectFace;                     // 反射する面
	float restitution;                       // 反射率.1.0fで減衰しない
	std::optional<Vector3> gravityDirection; // 重力のかかる方向
	std::optional<float> gravityStrength;    // 重力の強さ
};
struct ParticleData {

	ParticleTransform transform;
	Vector3 prePos;
	Vector3 velocity;
	Color color;
	float lifeTime;
	float currentTime;
	std::optional<float> easedLifeRatio;
	std::optional<EasingType> easingType;

	ParticlePhysics physics;

	Matrix4x4 worldMatrix;
	Matrix4x4 wvpMatrix;
};