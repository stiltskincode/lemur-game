#ifndef GENERALMENU_H_
#define GENERALMENU_H_

#include <GL/glew.h>

#include <string>
#include <fstream>

#include "GameEngine.h"
#include "EventManager.h"
#include "StringResources.h"
#include "TGAImage.h"
#include "SkinnedMesh.h"
#include "AnimationSet.h"
#include "SoundManager.h"
#include "Object3D.h"

class GeneralMenu : public Object3D
{
	public:
		GeneralMenu();
		virtual ~GeneralMenu();

		virtual void update();
		void setFont(std::string config, std::string texture);

		virtual void load(std::string location);
		virtual void reset() {active = true; start=end=1; elapsedTime=0; starting=true; ending=false; wait=0.5f;}
		virtual void draw();

	private:
		float diffX, diffY;
		int scrX, scrY;
		StringResources strings[4];
		SkinnedMesh mesh;
		AnimationSet anim;
		int cursor;
		GLuint tex, tex2, tex3, tlo;

		float start, end, elapsedTime, wait;
		bool starting, ending;
};

#endif
