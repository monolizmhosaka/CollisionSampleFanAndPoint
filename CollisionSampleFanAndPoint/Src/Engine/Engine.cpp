#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "Engine.h"

bool InitEngine(int width, int height, const char* title_name)
{
	if (MakeWindow(width, height, title_name) == false)
	{
		return false;
	}

	if (InitGraphics() == false)
	{
		return false;
	}

	if (InitInput() == false)
	{
		return false;
	}

	return true;
}

void EndEngine()
{
	EndGraphics();
	EndInput();
}
