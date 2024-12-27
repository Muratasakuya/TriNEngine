#pragma once

//============================================================================*/
//	include
//============================================================================*/

// c++
#include <cstdint>
#include <array>

//============================================================================*/
//	BlendMode
//============================================================================*/

enum BlendMode {

	kBlendModeNone,     // ブレンド無し
	kBlendModeNormal,   // 通常αブレンド
	kBlendModeAdd,      // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply, // 乗算
	kBlendModeScreen,   // スクリーン

	BlendCount
};
constexpr std::array<BlendMode, static_cast<size_t>(BlendMode::BlendCount)>
CreateBlendModeTypes() {
	std::array<BlendMode, static_cast<size_t>(BlendMode::BlendCount)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(BlendMode::BlendCount); ++i) {
		types[i] = static_cast<BlendMode>(i);
	}
	return types;
}
static constexpr uint32_t blendModeNum = static_cast<uint32_t>(BlendMode::BlendCount);
static constexpr
std::array<BlendMode, static_cast<size_t>(BlendMode::BlendCount)>
blendModeTypes = CreateBlendModeTypes();

//============================================================================*/
//	PipelineTypes
//============================================================================*/

enum PostProcessPipelineType
	: uint32_t {

	CopyTexture,
	Vignette,
	RadialBlur,

	PostProcessCount
};
constexpr std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::PostProcessCount)>
CreatePostProcessPipelineTypes() {
	std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::PostProcessCount)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(PostProcessPipelineType::PostProcessCount); ++i) {
		types[i] = static_cast<PostProcessPipelineType>(i);
	}
	return types;
}
static constexpr uint32_t postProcessPipelineNum = static_cast<uint32_t>(PostProcessPipelineType::PostProcessCount);
static constexpr
std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::PostProcessCount)>
postProcessPipelineTypes = CreatePostProcessPipelineTypes();

//=====================================================================================================================================*/

enum RendererPipelineType
	: uint32_t {

	Object2D,
	NormalObject3D,       //* 影になるObject
	TargetShadowObject3D, //* 影を落としたいObject
	PrimitiveLine,
	NormalParticle,

	RendererCount
};
constexpr std::array<RendererPipelineType, static_cast<size_t>(RendererPipelineType::RendererCount)>
CreateRendererPipelineTypes() {
	std::array<RendererPipelineType, static_cast<size_t>(RendererPipelineType::RendererCount)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(RendererPipelineType::RendererCount); ++i) {
		types[i] = static_cast<RendererPipelineType>(i);
	}
	return types;
}
static constexpr uint32_t rendererPipelineNum = static_cast<uint32_t>(RendererPipelineType::RendererCount);
static constexpr
std::array<RendererPipelineType, static_cast<size_t>(RendererPipelineType::RendererCount)>
rendererPipelineTypes = CreateRendererPipelineTypes();

//=====================================================================================================================================*/

enum ShadowPipelineType
	: uint32_t {

	ShadowDepth,

	ShadowCount
};
constexpr std::array<ShadowPipelineType, static_cast<size_t>(ShadowPipelineType::ShadowCount)>
CreateShadowPipelineTypes() {
	std::array<ShadowPipelineType, static_cast<size_t>(ShadowPipelineType::ShadowCount)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(ShadowPipelineType::ShadowCount); ++i) {
		types[i] = static_cast<ShadowPipelineType>(i);
	}
	return types;
}
static constexpr uint32_t shadowPipelineNum = static_cast<uint32_t>(ShadowPipelineType::ShadowCount);
static constexpr
std::array<ShadowPipelineType, static_cast<size_t>(ShadowPipelineType::ShadowCount)>
shadowPipelineTypes = CreateShadowPipelineTypes();

//=====================================================================================================================================*/

// ComputeShader
enum ComputePipelineType {

	SkinningCS,

	ComputeCount
};
constexpr std::array<ComputePipelineType, static_cast<size_t>(ComputePipelineType::ComputeCount)>
CreateComputePipelineTypes() {
	std::array<ComputePipelineType, static_cast<size_t>(ComputePipelineType::ComputeCount)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(ComputePipelineType::ComputeCount); ++i) {
		types[i] = static_cast<ComputePipelineType>(i);
	}
	return types;
}
static constexpr uint32_t computePipelineNum = static_cast<uint32_t>(ComputePipelineType::ComputeCount);
static constexpr
std::array<ComputePipelineType, static_cast<size_t>(ComputePipelineType::ComputeCount)>
computePipelineTypes = CreateComputePipelineTypes();

//=====================================================================================================================================*/