#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// c++
#include <string>

//============================================================================*/
// enum class

enum class ParticleRenderTarget {

	GameScene, // 実際のゲーム画面
	DemoScene  // Edit用の画面
};

//============================================================================*/
//	IBaseParticle class
//============================================================================*/
class IBaseParticle {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	IBaseParticle() = default;
	virtual ~IBaseParticle();

	virtual void Init(const std::string& particleName, const std::string modelName,
		const std::string textureName, ParticleRenderTarget renderTarget) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void ImGui() = 0;

	//* getter *//

	virtual std::string GetName() const { return name_; };

	//* setter *//

	virtual void SetParticleRenderer(const std::string& name, ParticleRenderTarget renderTarget);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::string name_;

};