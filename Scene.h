#ifndef SCENE_H_
#define SCENE_H_

#include "Monster.h"
#include "Blockade.h"
#include "Stage.h"
#include "LemurCamera.h"
#include "GameEngine.h"
#include "EventManager.h"
#include "CollisionDetector.h"

class Scene : public Object3D
{
	public:
		Scene();
		virtual ~Scene();

		virtual void load(std::string location);
		virtual void reset();
		virtual void draw();
		virtual void update();
		virtual void clear()
		{
			stage.clear();
			monsters.clear();
		}
		void start();

		Stage stage;
		LemurCamera cam;
		std::deque<Monster> monsters;
		std::deque<Blockade> blockades;
		bool starting;
		Mesh skydome;
};

#endif /* SCENE_H_ */
