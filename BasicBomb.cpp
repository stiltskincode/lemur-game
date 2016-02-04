#include "BasicBomb.h"

#include <iostream>
using namespace std;

Mesh BasicBomb::explosionMesh;
Mesh BasicBomb::meterMesh;
Vec3 *BasicBomb::cameraFront = NULL;
bool BasicBomb::created = false;

void BasicBomb::init(Vec3 *front)
{
	cameraFront=front;
}

bool BasicBomb::create(std::string location)
{
	if(created) return false;

	if(!bombMesh.load(location+"/bombMesh.dim",location)) return false;
	if(!explosionMesh.load(location+"/explosionMesh.dim",location)) return false;
	if(!meterMesh.load(location+"/wskaznik.dim")) return false;

	Explosion::init(delta,stage);

	SoundManager::addSound(location+"/bomb.wav","BasicBomb");

	created = true;

	return true;
}

BasicBomb::BasicBomb(Vec3 position, float life)
	:Bomb(position,life)
{
	range = 10;
	explosion.create(&explosionMesh, range);
	baseLife = life;
	angle = 0;
	if(cameraFront)
	{
		Vec3 x(1,0,0);
		angle = acos(dot(x,(*cameraFront)))*180.0f/3.141593f;
		if((*cameraFront)[2] < 0) angle+=90;
		else angle=90-angle;

		angle+=180;
	}

	active = true;
}

BasicBomb::~BasicBomb()
{
}

void BasicBomb::update()
{
	if(active) life -= *delta;

	explosion.update();

	if(life<=0 && !exploding)
	{
		explosion.explode(Vec3(position[0],50,position[2]),0.2f);
		SoundManager::stop("BasicBomb");
		SoundManager::play("BasicBomb");

		exploding=true;
	}

	if(life<=0 && explosion.done) active = false;
}

void BasicBomb::draw()
{
	if(life>0)
	{
		glPushMatrix();
			glTranslatef(position[0],0,position[2]);
			glRotatef(angle,0,1,0);
			glColor3f(1,1,1);
			bombMesh.draw();
			glColor3f(1-(life/baseLife),(life/baseLife),0);
			meterMesh.draw();
		glPopMatrix();
	}

	explosion.draw();
}
