#ifndef EXPLOSIONELEMENT_H_
#define EXPLOSIONELEMENT_H_

#include "Mesh.h"

class ExplosionElement
{
	public:
		ExplosionElement();
		virtual ~ExplosionElement();

		static void init(float *d);
		void create(Mesh *mesh);
		void explode(Vec3 position, float duration, float fullSizeDuration=0.0f);
		void draw();

	//private:
		Mesh *mesh;
		Vec3 position;
		float duration, elapsedTime, fullSizeDuration;
		bool created,active;

		static float *delta;
};

#endif /* EXPLOSIONELEMENT_H_ */
