#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_OpenGL.h>
#include <SDL_syswm.h>

#include <fstream>
#include <iostream>
#include <deque>
#include <vector>

#include "Object3D.h"
#include "LogWriter.h"
#include "EventManager.h"

#include "Dimental.h"
#include "GeneralMenu.h"
#include "Authors.h"
#include "Scene.h"
#include "Character.h"
#include "Layout.h"
//#include "StageEditor.h"

using namespace std;

class GameEngine
{
	public:

		struct Desc
		{
			std::string name;
			std::string location;
			std::string next;
			bool shared;
		};

	public:
		static void init();
		static void run();
		static void cleanup();

	//private:
		static bool initialized;
		static float start, end, delta;

		static int scrWidth, scrHeight;
		static Object3D *current;
		static deque<Object3D*> shared;
		static vector<Desc> desc;

		static Character lemur;

		static Layout layout;

		static void loadSharedObjects();
		static void loadDesc(std::string fileName);
		static bool getElement(std::string name);

		GameEngine();	//Prywatny konstruktor.
};

#endif /* GAMEENGINE_H_ */
