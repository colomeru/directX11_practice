#include <d3d11.h>
#include <crtdbg.h>
#include "GameBase.h"

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE, LPSTR, INT)
{
	// メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	GameBase game;

	if (!game.Run(hIns))
	{
		return -1;
	}

	return 0;
}