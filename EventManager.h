#pragma once


#include <SDL.h>

class EventManager
{
	public:
		static void getEvents();
		static SDL_Event* eventOccured(Uint8 type);
		static bool keyDown(SDLKey key);
		static bool keyUp(SDLKey key);
		static bool keyPressed(SDLKey key);
		static bool mouseButtonDown(int button);

		static int mouseX, mouseY, mouseRelX, mouseRelY;

	//private:
		static SDL_Event events[256];
		static bool down[296], up[296];
		static unsigned int num;
		static Uint8 *keyState, mouseState;

		EventManager();
};
