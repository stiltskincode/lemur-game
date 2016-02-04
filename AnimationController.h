#ifndef ANIMATIONCONTROLLER_H_
#define ANIMATIONCONTROLLER_H_

#define MAX_ELEMENTS 10

#include "AnimationSet.h"
#include "AnimationBlender.h"

class AnimationController
{
	struct Element
	{
		std::string name;
		AnimationSet *aset;
		float maxWeight;
	};

	public:
		AnimationController();
		virtual ~AnimationController();
		void update(float delta);
		bool add(AnimationSet *a, std::string name, float maxWeight=1.0f);

		//void remove(std::string name);
		void play(std::string name, float time, bool loop);
		//void stop(std::string name);
		//void pause(std::string name);
		//AnimationSet* get(std::string name);

	//private:
		Element *elements[MAX_ELEMENTS];
		unsigned int num;
		unsigned int numBlended;
		Element *incr;
		float blendDuration;	//Czas trwania blendingu.
		float elapsedTime;		//Czas, jaki juz uplynal.
		float value;			//Wartosc jaka musi zostac dodana do wagi.
		float startWeight;
};

#endif /*ANIMATIONCONTROLLER_H_*/
