
//===================================================================*/
//	include
//===================================================================*/
#include <Engine/Base/TriNFramework.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<TriNFramework> game = std::make_unique<TriNFramework>();
	game->Run();

	return 0;
}