#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <GL/glew.h>

#include <SDL.h>
#include "SkinnedMesh.h"
#include "AnimationSet.h"
#include "AnimationBlender.h"
#include "EventManager.h"
#include <fstream>
#include "BasicBomb.h"

#define MAX_CHARACTER_BOMBS 16

class Character : public Object3D
{
	public:

		enum Type
		{
			stand,
			moveForward,
			moveBackward,
			turnLeft,
			turnRight,
			strafeLeft,
			strafeRight,
			jump,
			die,
			run,
			putBomb,
			none
		};

		Character();
		virtual ~Character();
		virtual void handleInput();
		virtual void load(std::string location);
		virtual void update();
		virtual void draw();

		void setSuit(std::string location)
		{
			if(suit) delete suit;
			suit = new SkinnedMesh;
			suit->load(location+"/mesh.dim");
			suit->loadSkinningShader(location+"/vertex.vs");
			suit->setSkeleton(mesh.skeleton);

		}
		virtual void kill() {dying=true;}
		virtual void rotateRight();
		virtual void rotateLeft();
		virtual void walkForward();

		virtual void rotateY(float angle)
		{
			front.rotate(0,angle,0);
			right=cross(front,up);
			mesh.getFront().rotate(0,angle,0);
			Mat4 r; r.rotationY(angle);
			mesh.tmat*=r;
		}

		virtual void setPosition(float x, float y, float z)
		{
			mesh.setPosition(x,y,z);
		}

	//private:
		SkinnedMesh mesh;
		float speed;
		bool initialized, initAction, actionLasts;
		SDLKey keys[11];
		AnimationSet anims[11], *a, *b, *actionA, *actionB;
		AnimationBlender blender;
		Type actionType;
		float rotationAngle;

		bool mouseLeft, mouseRight;
		float resetMouse, rotationSpeed;
		bool alive, dying;

		bool forceWalkForward, forceAction;

		int bombsLeft, bombsLimit;
		Bomb *bombs[MAX_CHARACTER_BOMBS];

		bool readKeyConfig(std::string fileName);
		SDLKey keyValue(std::string value);

	//private:
		Uint8 *keystate;

	private:
		SkinnedMesh *suit;
};

#endif
