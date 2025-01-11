#include "ParticleSystem.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/Asset.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	ParticleSystem classMethods
//============================================================================*/

void ParticleSystem::CreateVertexData(const std::string& name) {

	size_t meshNum = particleGroups_[name].model.data.meshes.size();

	for (uint32_t index = 0; index < meshNum; ++index) {

		particleGroups_[name].model.inputAssembler.Init(particleGroups_[name].model.data.meshes[index]);
	}

}

Matrix4x4 ParticleSystem::CalBillboardMatrix() const {

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix =
		Matrix4x4::Multiply(backToFrontMatrix, GameSystem::GameCamera()->GetCamera3D()->GetCameraMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	return billboardMatrix;
}

void ParticleSystem::Update() {

	Matrix4x4 billboardMatrix = CalBillboardMatrix();
	for (auto& [name, group] : particleGroups_) {

		auto& particles = group.particles;
		group.particleBuffer.properties.resize(particles.size());

		for (auto it = particles.begin(); it != particles.end();) {

			// 寿命が切れたら削除
			if (it->lifeTime <= it->currentTime) {

				it = particles.erase(it);
				continue;
			}

			group.behavior->Update(*it, billboardMatrix);

			int index = static_cast<uint32_t>(std::distance(particles.begin(), it));
			if (index < group.particleBuffer.properties.size()) {

				group.particleBuffer.properties[index].World = it->worldMatrix;
				group.particleBuffer.properties[index].WVP = it->wvpMatrix;
				group.particleBuffer.properties[index].color = it->color;
			}

			++it;
		}

		group.numInstance = static_cast<uint32_t>(particles.size());
		group.particleBuffer.Update();
	}

}

void ParticleSystem::Draw(const std::string& name, BlendMode blendMode) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	for (uint32_t index = 0; index < particleGroups_[name].model.data.meshes.size(); ++index) {

		GraphicsEngine::GetPipeline()->SetRendererPipeline(commandList, NormalParticle, blendMode);
		particleGroups_[name].model.inputAssembler.SetBuffer(commandList, index);
		particleGroups_[name].particleBuffer.SetCommand(commandList);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Asset::GetTexture()->GetTextureGPUHandle(particleGroups_[name].model.data.meshes[index].textureName.value());
		commandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
		commandList->SetGraphicsRootDescriptorTable(2, GraphicsEngine::SRV()->GetGPUHandle(particleGroups_[name].instancingSrvIndex));
		particleGroups_[name].model.inputAssembler.DrawCall(commandList, particleGroups_[name].numInstance, index);
	}
}

void ParticleSystem::CreateParticle(
	const std::string& modelName, const std::string& name,
	ParticleType particleType, ParticleParameter& parameter) {

	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	particleGroups_[name].behavior = ParticleBehaviorFactory::CreateBehavior(particleType);

	particleGroups_[name].behavior->Create(particleGroups_[name].particles, parameter);

	// 最初は0.0fにして暴発しないようにする
	for (auto& particle : particleGroups_[name].particles) {
		particle.lifeTime = 0.0f;
	}

	//!! alreadyLoadModel !!//
	particleGroups_[name].model.data = Asset::GetModel()->GetModelData(modelName);
	CreateVertexData(name);
	particleGroups_[name].model.data.meshes
		[static_cast<int>(particleGroups_[name].model.data.meshes.size() - 1)].textureName =
		Asset::GetModel()->GetModelData(modelName).meshes.front().textureName.value_or("assert");

	//* CreateStructureBuffer *//
	particleGroups_[name].srvIndex = GraphicsEngine::SRV()->Allocate();
	particleGroups_[name].numInstance = instanceMaxCount_;
	particleGroups_[name].particleBuffer.Init(particleGroups_[name].numInstance);
	particleGroups_[name].instancingSrvIndex = particleGroups_[name].srvIndex;
	GraphicsEngine::SRV()->CreateSRVForStructureBuffer(
		particleGroups_[name].instancingSrvIndex, particleGroups_[name].particleBuffer.GetResource(),
		particleGroups_[name].numInstance, sizeof(ParticleForGPU));
}

void ParticleSystem::EmitParticle(
	const std::string& name, ParticleType particleType, ParticleParameter& parameter) {

	auto it = particleGroups_.find(name);
	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		if (group.behavior->GetType() == particleType) {

			particleGroups_[name].behavior->Create(particleGroups_[name].particles, parameter);
		} else {

			assert(false && "behavior type of the existing particle group does not match");
		}
	}
}

void ParticleSystem::SetTexture(const std::string& name, const std::string& textureName) {

	particleGroups_[name].model.data.meshes
		[static_cast<int>(particleGroups_[name].model.data.meshes.size() - 1)].textureName = textureName;
}