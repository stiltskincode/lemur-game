#ifndef DIMENTAL_H_
#define DIMENTAL_H_

#include "GameEngine.h"
#include "Object3D.h"
#include "TGAImage.h"

class Dimental: public Object3D
{
	public:
		Dimental();
		virtual ~Dimental();

		virtual void draw();
		virtual void load(std::string location);
		virtual void reset();

		GLuint tex;
		float elapsedTime, duration, start, stop;
};

#endif /* DIMENTAL_H_ */
