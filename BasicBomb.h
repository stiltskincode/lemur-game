#ifndef BASICBOMB_H_
#define BASICBOMB_H_

#include "Explosion.h"
#include "Bomb.h"
#include "SoundManager.h"

class BasicBomb: public Bomb
{
	public:
		BasicBomb(Vec3 position, float life);
		virtual ~BasicBomb();

		virtual void update();
		virtual void draw();

		static void init(Vec3 *front);
		static bool create(std::string location);

	protected:
		Explosion explosion;
		int range;
		float baseLife, angle;

	private:
		static Mesh explosionMesh;
		static Mesh meterMesh;
		static Vec3 *cameraFront;
		static bool created;
};

#endif /* BASICBOMB_H_ */
