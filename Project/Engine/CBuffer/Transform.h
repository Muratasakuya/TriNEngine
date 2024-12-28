#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector2.h>
#include <Lib/MathUtils/Quaternion.h>
#include <Engine/Utility/AssetStructure.h>

// c++
#include <utility>
#include <cstdint>
#include <unordered_map>
#include <optional>

//============================================================================*/
//	bufferSize Transform
//============================================================================*/
struct TransformationMatrix {

	Matrix4x4 world;
	Matrix4x4 wvp;
	Matrix4x4 worldInverseTranspose;
};

struct Transform2D {

	Vector2 pos;            // 座標
	float rotate;           // 回転
	Vector2 size;           // 大きさ
	Vector2 anchorPoint;    // アンカーポイント /* {0.5f,0.5f}で真ん中 */
	Vector2 textureLeftTop; // テクスチャ左上座標
	Vector2 textureSize;    // テクスチャ切り出しサイズ
	bool isFlipX;           // 左右反転
	bool isFlipY;           // 上下反転

	void Init() {

		pos = { 0.0f,0.0f };
		rotate = 0.0f;
		size = { 0.0f,0.0f };
		anchorPoint = { 0.0f,0.0f };
		textureLeftTop = { 0.0f,0.0f };
		textureSize = { 0.0f,0.0f };
		isFlipX = false;
		isFlipY = false;
	}

};

//============================================================================*/
//	BaseTransform class
//============================================================================*/
class BaseTransform :
	public DXConstBuffer<TransformationMatrix> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseTransform() = default;
	virtual ~BaseTransform() = default;

	virtual void Init();
	virtual void Update() = 0;

	//* getter *//

	virtual Vector3 GetWorldPos() const;

	//========================================================================*/
	//* variables

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;

	TransformationMatrix matrix;

};


//============================================================================*/
//	WorldTransform class
//============================================================================*/
class WorldTransform :
	public BaseTransform {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	WorldTransform() = default;
	~WorldTransform() = default;

	void Update() override;

	//========================================================================*/
	//* variables

	const WorldTransform* parent = nullptr;

};

//============================================================================*/
//	AnimationTransform class
//============================================================================*/
class AnimationTransform :
	public BaseTransform {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	AnimationTransform() = default;
	~AnimationTransform() = default;

	void Init(const std::string& modelName, const std::string& animationName);

	void Update() override;

	void AnimationInfo();

	//* setter *//

	void SetPlayAnimation(bool isPlayAnimation, const std::string& animationName);

	void SetNewAnimationData(const std::string& animationName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ModelData modelData_;

	std::unordered_map<std::string, AnimationData> animationData_;
	std::unordered_map<std::string, std::optional<Skeleton>> skeleton_;
	std::unordered_map<std::string, SkinCluster> skinCluster_;

	std::pair<bool, std::string> animationController_;
	float animationTime_;

};

//============================================================================*/
//	SpriteTransformBuffer class
//============================================================================*/
class SpriteTransformBuffer :
	public DXConstBuffer<Matrix4x4> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SpriteTransformBuffer() = default;
	~SpriteTransformBuffer() = default;

	void Init();

	void Update(const Transform2D& transform);

};