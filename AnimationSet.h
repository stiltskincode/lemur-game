#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <string>
#include <fstream>
#include "Object.h"
#include "Skeleton.h"
#include "Mathematics.h"

typedef unsigned int UINT;

struct Key
{
	unsigned int time;	//Czas.
	Quat r;				//Rotacja.
	Vec3 s;				//Skalowanie.
	Vec3 t;				//Translacja.
};

struct Animation
{
	Skeleton::Bone *bone;		//Kosc, ktora modyfikuje animacja.
	std::string name, bname;	//Nazwa animacji i kosci.
	unsigned int num;			//Ilosc kluczy animacji.
	Key *keys;					//Zbior kluczy animacji.
};

class AnimationSet : public Object
{
	friend class AnimationController;

	public:
		AnimationSet();
		AnimationSet(const AnimationSet& pattern);
		~AnimationSet();

		bool load(std::string fileName);
		void bind(Skeleton *s);
		void update(float delta);
		void updateSkeleton();
		void setWeight(float weight);

		void play() {stopped=paused=finished=false;}
		void stop() {stopped=true; paused=false;}
		void pause(){stopped=false; paused=true;}
		void setLoop(bool value) {looped=value;}
		bool isLooped() {return looped;}
		bool isStopped(){return stopped;}
		bool isPaused() {return paused;}
		bool isFinished() {return finished;}

		AnimationSet& operator=(const AnimationSet& pattern);

		void reset();

	//protected:

		unsigned int num;	//Ilosc animacji w zbiorze.
		Animation *anims;	//Zbior animacji.

		unsigned int duration;	//Czas trwania zbioru.
		unsigned int lastK1;	//Ostatni znaleziony klucz k1.
		Skeleton *skeleton;		//Szkielet modyfikowany przez zbior.
		float weight, elapsedTime;		//Waga i czas.

		bool stopped, paused, looped, finished;
		bool loaded, bounded;
		SRT **srt;
};

#endif /*ANIMATIONSET_H_*/
