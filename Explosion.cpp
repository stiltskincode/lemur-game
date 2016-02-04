#include "Blockade.h"
#include "Explosion.h"

float *Explosion::delta=NULL;
Stage *Explosion::stage=NULL;

void Explosion::init(float *d, Stage *s)
{
	delta=d;
	stage=s;

	ExplosionElement::init(d);
}


Explosion::Explosion()
{
	active=created=done=false;
	ee[0]=ee[1]=ee[2]=ee[3]=NULL;
	range=index=0;
	elapsedTime=period=0.0f;

	for(int i=0; i<4; i++) directions[i]=false;
}

Explosion::~Explosion()
{
	for(int i=0; i<4; i++) if(ee[i]) {delete[] ee[i]; ee[i]=NULL;}
}

bool Explosion::create(Mesh *mesh, int range)
{
	this->range = range;
	this->position = position;

	for(int i=0; i<4; i++)
	{
		ee[i] = new ExplosionElement[range];
		for(int j=0; j<range; j++) ee[i][j].create(mesh);
	}

	created=true;
	deleteMesh=false;

	return true;
}

void Explosion::explode(Vec3 position, float period)
{
	if(created) active = true;
	else return;

	this->position = position;
	this->period = period;

	for(int i=0; i<4; i++) directions[i]=true;
}

void Explosion::update()
{
	if(!active) return;

	elapsedTime+=(*delta);

	if(elapsedTime>=period) {index++; elapsedTime=0; }

	if(index>range && !ee[0][index-1].active)
	{
		elapsedTime=index=0;
		active=false;
		done=true;
		return;
	}

	int x=(int)position[0]/200, z=(int)position[2]/200;

	//Tablice indeksow elementow planszy.
	int sz[4]={z,z-index,z,z+index}, sx[4]={x-index,x,x+index,x};

	for(int i=0; i<4; i++)
	{
		if(!directions[i]) continue;

		if(stage->stage[sz[i]][sx[i]].wall) directions[i]=false;
		else ee[i][index].explode( Vec3((sx[i]*200)+100,position[1],(sz[i]*200)+100),0.5,0.2);

		if(stage->stage[sz[i]][sx[i]].bomb)
		{
			((Bomb*)stage->stage[sz[i]][sx[i]].bomb)->explode();
			stage->stage[sz[i]][sx[i]].bomb=NULL;
		}
		if(stage->stage[sz[i]][sx[i]].monster)
		{
			//((Monster*)stage->stage[sz[i]][sx[i]].monster)->setActive(false);
			((Monster*)stage->stage[sz[i]][sx[i]].monster)->kill();
			stage->stage[sz[i]][sx[i]].monster=NULL;
		}
		if(stage->stage[sz[i]][sx[i]].blockade)
		{
			//((Monster*)stage->stage[sz[i]][sx[i]].monster)->setActive(false);
			((Blockade*)stage->stage[sz[i]][sx[i]].blockade)->kill();
			stage->stage[sz[i]][sx[i]].blockade=NULL;
			stage->stage[sz[i]][sx[i]].wall=false;
		}
	}
}

void Explosion::draw()
{
	for(int i=0; i<4; i++) for(int j=0; j<range; j++) ee[i][j].draw();
}
