#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/LightBuffer.h>

//============================================================================*/
//	LightManager class
//============================================================================*/
class LightManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	LightManager() = default;
	~LightManager() = default;

	void Init();

	void Update();

	void SelectGameLight();

	void ImGui();

	//* getter *//

	LightBuffer GetLightBuffer() const { return buffer_; }

	bool SelectedGameLight() const { return selectGameLight_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	LightBuffer buffer_;

	bool selectGameLight_;

};