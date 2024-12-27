#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Editor/Base/BaseEditor.h>

// c++
#include <vector>

//============================================================================*/
//	EditorManager class
//============================================================================*/
class EditorManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	EditorManager() = default;
	~EditorManager() = default;

	static void SetEditor(BaseEditor* editor);

	static void EraseEditor(BaseEditor* editor);

	static void Clear();

	//* imgui *//

	static void SelectEditor(const ImVec2& mainWindowPos);

	static void SelectedImGui();

	//* getter *//

	static BaseEditor* GetSelectedEditor() { return selectedEditor_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<BaseEditor*> editors_;

	static BaseEditor* selectedEditor_;

	static int currentEditorIndex_;

};