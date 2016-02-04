#ifndef AUTHORS_H_
#define AUTHORS_H_

#include "GameEngine.h"
#include "Object3D.h"
#include "TGAImage.h"
#include "BitmapText.h"
#include "EventManager.h"
#include "StringResources.h"

class Authors: public Object3D
{
	public:
		Authors();
		virtual ~Authors();

		virtual void load(std::string location);
		virtual void reset();
		virtual void draw();
		virtual void update();

	private:
		GLuint tex;
		//BitmapText bt;
		//BitmapFont bf;
		StringResources credits;
		float height;

		float alpha;
		bool starting, ending;
};

#endif /* AUTHORS_H_ */
