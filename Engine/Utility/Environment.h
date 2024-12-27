#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector4.h>

// c++
#include <cstdint>

//============================================================================*/
//	utilityEnvironment
//============================================================================*/

// windowSize
const constexpr uint32_t kWindowWidth = 1280;
const constexpr uint32_t kWindowHeight = 720;

// windowTitle
const constexpr wchar_t* kWindowTitle = L"Engine";

// doubleBuffer
const constexpr uint32_t kBufferCount = 2;

// windowClearColor
const Color kWindowClearColor = Color(0.032f, 0.032f, 0.032f, 1.0f);

// shadowMapSize
const constexpr uint32_t kShadowMapWidth = 1024;
const constexpr uint32_t kShadowMapHeight = 1024;

// gravity
const constexpr float gravity = -9.8f;

// groundY
const constexpr float groundY = 0.0f;