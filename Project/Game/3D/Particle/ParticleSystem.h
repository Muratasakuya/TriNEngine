#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/IBaseParticle.h>
#include <Engine/CBuffer/ParticleBuffer.h>
#include <Engine/CBuffer/InputAssembler.h>
#include <Game/3D/Particle/ParticleBehavior.h>
#include <Game/3D/Particle/ParticleParameter.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// c++
#include <unordered_map>
#include <optional>

//============================================================================*/
//	ParticleSystem class
//============================================================================*/
class ParticleSystem {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/
	
	//========================================================================*/
	///* structs

	//* ParticleModel *//
	struct ParticleModel {

		ModelData data;
		InputAssembler inputAssembler;
	};

	//* ParticleGroup *//
	struct ParticleGroup {

		ParticleModel model;
		std::list<ParticleData> particles;
		std::unique_ptr<ParticleBehavior> behavior;
		uint32_t srvIndex;
		uint32_t numInstance;
		uint32_t instancingSrvIndex;
		ParticleBuffer particleBuffer;
		ParticleRenderTarget renderTarget;
	};

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleSystem() = default;
	~ParticleSystem() = default;

	void Update();

	void Draw(const std::string& name, BlendMode blendMode);

	void CreateParticle(
		const std::string& modelName, const std::string& name,
		ParticleType particleType, ParticleParameter& parameter, ParticleRenderTarget renderTarget);

	void EmitParticle(
		const std::string& name, ParticleType particleType, ParticleParameter& parameter);

	void SetTexture(const std::string& name, const std::string& textureName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	///* variables

	const uint32_t instanceMaxCount_ = 512; //* Instancing

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	//========================================================================*/
	///* functions

	void CreateVertexData(const std::string& name);
	Matrix4x4 CalBillboardMatrix(ParticleRenderTarget renderTarget) const;

};