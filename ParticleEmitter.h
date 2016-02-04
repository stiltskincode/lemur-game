#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "Particle.h"
#include "Mathematics.h"
#include <iostream>

class ParticleEmitter
{
	public:
		ParticleEmitter();
		//ParticleEmitter(bool active, Particle pattern, int number, float period);
		virtual ~ParticleEmitter();

		void init(float *d);
		void create(bool active, Particle pattern, int number, float period);
		bool update();
		bool draw();

	//private:
		Particle pattern;
		Particle *par;
		float period, timeLeft;
		unsigned int number, numberParticles;
		bool active;
		float *delta;
		float t;

		bool initialized;
};

#endif
