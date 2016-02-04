#include "EventManager.h"
#include <iostream>

SDL_Event EventManager::events[256];
bool EventManager::down[296];
bool EventManager::up[296];
Uint8 *EventManager::keyState=NULL;
Uint8 EventManager::mouseState;
int EventManager::mouseX=0;
int EventManager::mouseY=0;
int EventManager::mouseRelX=0;
int EventManager::mouseRelY=0;
unsigned int EventManager::num=0;

void EventManager::getEvents()
{
	SDL_PumpEvents();

	keyState = SDL_GetKeyState(NULL);
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	for(int i=0; i<296; i++) up[i]=down[i]=false;

	num=0;
	mouseRelX=mouseRelY=0;
	while(SDL_PollEvent(&events[num]))
	{
		if(events[num].type == SDL_KEYDOWN)
			for(int i=0; i<296; i++) if(events[num].key.keysym.sym == SDLKey(i)) down[i]=true;

		if(events[num].type == SDL_KEYUP)
			for(int i=0; i<296; i++) if(events[num].key.keysym.sym == SDLKey(i)) up[i]=true;

		if(events[num].type == SDL_MOUSEMOTION)
		{
			mouseRelX=events[num].motion.xrel;
			mouseRelY=events[num].motion.yrel;
		}
		num++;
	}
}

SDL_Event* EventManager::eventOccured(Uint8 type)
{
	for(unsigned int i=0; i<num; i++) if(events[i].type == type) return &events[i];
	return NULL;
}

bool EventManager::keyDown(SDLKey key)
{
	if(down[key]) return true;
	else return false;
}

bool EventManager::keyUp(SDLKey key)
{
	if(up[key]) return true;
	else return false;
}

bool EventManager::keyPressed(SDLKey key)
{
	if(keyState[key]) return true;
	else return false;
}

bool EventManager::mouseButtonDown(int button)
{
	if(mouseState&SDL_BUTTON(button)) return true;
	else return false;
}
