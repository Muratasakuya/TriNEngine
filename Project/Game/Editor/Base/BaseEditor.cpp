#include "BaseEditor.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Editor/Manager/EditorManager.h>

//============================================================================*/
//	BaseEditor classMethods
//============================================================================*/

BaseEditor::~BaseEditor() {
	EditorManager::EraseEditor(this);
}

void BaseEditor::SetEditor(const std::string& name) {

	name_ = name;

	EditorManager::SetEditor(this);
}