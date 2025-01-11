#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/BaseParticle.h>
#include <Game/Editor/Base/BaseEditor.h>

// c++
#include <memory>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>

// using
using DispersionParticle = BaseParticle<ParticleType::kDispersion>;
using InjectionParticle = BaseParticle<ParticleType::kInjection>;
using ChaseParticle = BaseParticle<ParticleType::kChase>;

//============================================================================*/
//	ParticleEditor class
//============================================================================*/
class ParticleEditor :
	public BaseEditor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleEditor();
	~ParticleEditor() = default;

	void Update();

	//* imgui *//

	void ImGui() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::vector<std::unique_ptr<IBaseParticle>> particles_;

	//* add *//

	// particle
	ParticleType addParticleType_;
	std::string addParticleName_;

	// model
	std::string addModelName_;
	int addSelectedModelIndex_;

	// texture
	std::string addTextureName_;
	int addSelectedTextureIndex_;

	//* edit *//

	std::optional<IBaseParticle*> selectedParticle_;
	int currentParticleIndex_;

	//========================================================================*/
	//* functions

	void CreateAddParticle();

	template <typename ParticleType>
	void AddParticle(const std::string& name, const std::string& modelName, const std::string& textureName);

	std::string GetUniqueParticleName(
		const std::string& baseName, const std::vector<std::unique_ptr<IBaseParticle>>& particles);

};

template<typename ParticleType>
inline void ParticleEditor::AddParticle(
	const std::string& name, const std::string& modelName, const std::string& textureName) {

	auto particle = std::make_unique<ParticleType>();
	particle->Init(name, modelName, textureName, ParticleRenderTarget::DemoScene);
	particles_.emplace_back(std::move(particle));
}