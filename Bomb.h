#ifndef BOMB_H_
#define BOMB_H_

#include "Mesh.h"
#include "CollisionDetector.h"

#define MAX_BOMBS 100

class Stage;

class Bomb : public Object3D
{
	public:
		Bomb(Vec3 position, float life);
		virtual ~Bomb();

		virtual void update();
		virtual void draw();
		virtual void explode();

		static void drawBombs();
		static void create(std::string location);
		static void init(Stage *stage);
		static void release();

	protected:
		float life;
		int stageX, stageZ;
		bool exploding;

		static Stage *stage;
		static Mesh bombMesh;
		static Bomb *bombs[MAX_BOMBS];

	private:
		static bool created;
};

#endif /* BOMB_H_ */
