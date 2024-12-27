#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Matrix4x4.h>

//============================================================================*/
//	CameraBuffer class
//============================================================================*/
class CameraBuffer :
	public DXConstBuffer<Vector3> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	CameraBuffer() = default;
	~CameraBuffer() = default;

	void Init();

	void Update(const Vector3& translate);

};


//============================================================================*/
//	ViewProjectionBuffer class
//============================================================================*/
class ViewProjectionBuffer :
	public DXConstBuffer<Matrix4x4> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ViewProjectionBuffer() = default;
	~ViewProjectionBuffer() = default;

	void Init();

	void Update(const Matrix4x4& viewProjection);

};

//============================================================================*/
//	LightViewProjectionBuffer class
//============================================================================*/
class LightViewProjectionBuffer :
	public DXConstBuffer<Matrix4x4> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	LightViewProjectionBuffer() = default;
	~LightViewProjectionBuffer() = default;

	void Init();

	void Update(const Matrix4x4& viewProjection);

};