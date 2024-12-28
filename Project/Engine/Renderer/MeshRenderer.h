#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/IBaseGameObject.h>

// imgui
#include <imgui.h>

// c++
#include <vector>
#include <unordered_map>

//============================================================================*/
//	MeshRenderer class
//============================================================================*/
class MeshRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	MeshRenderer() = default;
	~MeshRenderer() = default;

	static void RenderShadowDepth();

	static void Render();

	static void SetGameObject(IBaseGameObject* gameObject);

	static void EraseGameObject(IBaseGameObject* gameObject);

	static void Clear();

	//* imgui *//

	static void SelectGameObject();

	static void SelectedImGui();

	//* getter *//

	static IBaseGameObject* GetSelectedObject() { return selectedGameObject_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<IBaseGameObject*> gameObjects_;

	static IBaseGameObject* selectedGameObject_;

	static int currentObjectIndex_;

};