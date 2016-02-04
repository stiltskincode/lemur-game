#include "Bomb.h"

Bomb *Bomb::bombs[MAX_BOMBS];
Stage *Bomb::stage = NULL;
Mesh Bomb::bombMesh;
bool Bomb::created = false;

void Bomb::init(Stage *stage)
{
	Bomb::stage = stage;

	for(int i=0; i<MAX_BOMBS; i++) bombs[i] = NULL;
}

void Bomb::create(std::string location)
{
	created = bombMesh.load(location+"/bombMesh.dim",location);
}

void Bomb::release()
{
	for(int i=0; i<MAX_BOMBS; i++)
	{
		if(!bombs[i]) continue;
		delete bombs[i];
		bombs[i] = NULL;
	}
}

void Bomb::drawBombs()
{
	for(int i=0; i<MAX_BOMBS; i++)
	{
		if(!bombs[i]) continue;
		bombs[i]->update();
		bombs[i]->draw();
	}
}

Bomb::Bomb(Vec3 position, float life)
{
	active=false;
	exploding=false;

	stageX=int(position[0]/200);
	stageZ=int(position[2]/200);

	if(created && !stage->stage[stageZ][stageX].bomb)
	{
		this->life = life;
		active=true;

		this->position.xyz( (stageX*200)+100, 0, (stageZ*200)+100);
		stage->stage[stageZ][stageX].bomb = this;

		for(int i=0; i<MAX_BOMBS; i++)
		{
			if(bombs[i]) continue;
			bombs[i] = this;
			bombs[i]->setID(i);
			CollisionDetector::add(&this->position,"bomb"+i,70,1,false);
			//cout<<this->position[0]<<" "<<this->position[1]<<" "<<this->position[2]<<endl;
			break;
		}
	}
}

Bomb::~Bomb()
{
	bombs[id] = NULL;
}

void Bomb::update()
{
	if(!active) return;

	life -= *delta;
	if(life<=0) active = false;
}

void Bomb::draw()
{
	if(!active) return;

	glPushMatrix();
		glTranslatef(position[0],0,position[2]);
		bombMesh.draw();
	glPopMatrix();
}

void Bomb::explode()
{
	life=0;
	CollisionDetector::remove("bomb"+id);
}
