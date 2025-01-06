#pragma once

#include "ParticleParameter.h"
#include "ParticleBehavior.h"

//============================================================================*/
//	ParticleTraits
//============================================================================*/

template <ParticleType>
struct ParticleTraits;

/// <summary>
/// DispersionParticle
/// </summary>
template <>
struct ParticleTraits<kDispersion> {

	using ParameterType = DispersionParticleParameter;
};

/// <summary>
/// ChaseParticle
/// </summary>
template <>
struct ParticleTraits<kChase> {

	using ParameterType = ChaseParticleParameter;
};

/// <summary>
/// ConvergeParticle
/// </summary>
template <>
struct ParticleTraits<kConverge> {

	using ParameterType = ConvergeParticleParameter;
};

/// <summary>
/// InjectionParticle
/// </summary>
template <>
struct ParticleTraits<kInjection> {

	using ParameterType = InjectionParticleParameter;
};