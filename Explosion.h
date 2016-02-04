#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Bomb.h"
#include "Camera.h"
#include "ExplosionElement.h"
#include "Stage.h"
#include <string>

class Stage;

class Explosion
{
	public:
		Explosion();
		virtual ~Explosion();

		static void init(float *delta, Stage *stage);

		bool create(Mesh *mesh, int range);
		void explode(Vec3 position, float duration);
		void update();
		void draw();

	//private:

		ExplosionElement *ee[4];
		Vec3 position;
		int range, index;
		bool active, created, deleteMesh, done;
		bool directions[4];
		float elapsedTime, period;

		static Stage *stage;
		static float *delta;
};

#endif
