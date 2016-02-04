#include "ParticleEmitter.h"

using namespace std;

ParticleEmitter::ParticleEmitter()
{
	t=0;
	active=true;
}

ParticleEmitter::~ParticleEmitter()
{
	if(par) delete[] par; par=NULL;
}

void ParticleEmitter::init(float *d)
{
	delta=d;
	initialized=true;
}

void ParticleEmitter::create(bool active, Particle pattern, int number, float period)
{
	timeLeft=this->period=period;
	this->number=number;
	this->pattern=pattern;
	numberParticles = int((number/period)*pattern.life);
	par = new Particle[numberParticles];
	this->active=active;

	for(unsigned int i=0; i<numberParticles; i++)
	{
		//par[i] = pattern;
		par[i].active=false;
	}
	//cout << endl << "dupa";
}

bool ParticleEmitter::update()
{
	if(!active || !initialized) return false;

	for(unsigned int i=0; i<numberParticles; i++)
	{
		if(!par[i].active) continue;
		par[i].update();
	}

	//cout << endl << "pufa";

	timeLeft-=(*delta);

	//t+=(*delta);
	//if(t>=0.5)
	//{
	//	t=0;
	//	active=false;
	//}

	unsigned int num=number;	//Ilosc jaka nalezy wygenerowac.

	if(timeLeft<=0)
	{
		for(unsigned int j=0; j<numberParticles; j++)
		{
			if(par[j].active) continue;

			par[j]=pattern;
			num--;
			if(num<=0) break;
		}
		timeLeft=period;
	}

	return true;
}

bool ParticleEmitter::draw()
{
	if(!active || !initialized) return false;

	for(unsigned int i=0; i<numberParticles; i++)
		if(par[i].active) par[i].draw();

	return true;
}
