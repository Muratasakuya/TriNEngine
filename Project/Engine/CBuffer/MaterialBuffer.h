#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Vector4.h>
#include <Lib/MathUtils/Matrix4x4.h>

// c++
#include <optional>

//============================================================================*/
//	bufferSize
//============================================================================*/
struct Material {

	Color color;
	int32_t enableLighting;
	int32_t enableHalfLambert;
	int32_t enablePhongReflection;
	int32_t enableBlinnPhongReflection;
	float phongRefShininess;
	Vector3 specularColor;
	Matrix4x4 uvTransform;

	void Init() {

		color = Color::White();
		enableLighting = false;
		enableHalfLambert = false;
		enablePhongReflection = false;
		enableBlinnPhongReflection = false;
		phongRefShininess = 1.0f;
		specularColor = Vector3(1.0f, 1.0f, 1.0f);
		uvTransform = Matrix4x4::MakeIdentity4x4();
	}

	void SetUVTransform(
		const std::optional<Vector3>& scale = std::nullopt,
		const std::optional<Vector3>& rotate = std::nullopt,
		const std::optional<Vector3>& translate = std::nullopt) {

		uvTransform =
			Matrix4x4::MakeAffineMatrix(
				scale.value_or(Vector3(1.0f, 1.0f, 1.0f)),
				rotate.value_or(Vector3(0.0f, 0.0f, 0.0f)),
				translate.value_or(Vector3(0.0f, 0.0f, 0.0f)));
	};
};

struct SpiteMaterial {

	Color color;
	Matrix4x4 uvTransform;

	void Init() {

		color = Color::White();;
		uvTransform = Matrix4x4::MakeIdentity4x4();
	}
};

//============================================================================*/
//	PrimitiveMaterial class
//============================================================================*/
class PrimitiveMaterial :
	public DXConstBuffer<Color> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PrimitiveMaterial() = default;
	~PrimitiveMaterial() = default;

	void Init();

	void Update();

	//========================================================================*/
	//* variables

	Color color;

};

//============================================================================*/
//	MaterialBuffer class
//============================================================================*/
class MaterialBuffer :
	public DXConstBuffer<Material> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	MaterialBuffer() = default;
	~MaterialBuffer() = default;

	void Init();

	void Update();

	//========================================================================*/
	//* variables

	Material properties;

};

//============================================================================*/
//	SpriteMaterialBuffer class
//============================================================================*/
class SpriteMaterialBuffer :
	public DXConstBuffer<SpiteMaterial> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SpriteMaterialBuffer() = default;
	~SpriteMaterialBuffer() = default;

	void Init();

	void Update();

	//========================================================================*/
	//* variables

	SpiteMaterial properties;

};