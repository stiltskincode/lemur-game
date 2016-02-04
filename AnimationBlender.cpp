#include "AnimationBlender.h"


#include <sstream>
#include <SDL.h>

using namespace std;

AnimationBlender::AnimationBlender()
{
	reset();
}

AnimationBlender::~AnimationBlender()
{
	reset();
}

/**********************************************************************
 Przywraca 'fabryczne' ustawienia blender'a.
**********************************************************************/
void AnimationBlender::reset()
{
	a=b=c=NULL;
	elapsedTime=duration=0.0f;
	done=true;
	this->blendBack=false;
}

/**********************************************************************
 Plynnie 'miesza' nowa animacje 'a' z juz istniejaca animacja 'b'.
 Dzialanie polega na stopniowym zwiekszaniu wagi animacji 'a' tak, by
 osiagnela ona poziom rowny 'l' w czasie 'd'.
**********************************************************************/
bool AnimationBlender::blend(AnimationSet *a, AnimationSet *b, AnimationSet *c, float duration, bool blendBack)
{
	//Blokujemy mozliwosc inicjalizacji blendingu, jezeli podano puste wskazniki
	//do animacji a lub b, jezeli blender znajduje sie w trakcie dzialania lub
	//animacje podane jako argumenty posiadaja finalne wartosci.
	if( !a || !b || a->weight==1.0f || b->weight==0.0f || this->a || this->b ) return false;

	this->a=a; this->b=b; this->c=(c==NULL)? b:c;
	this->duration=duration;
	this->blendBack = blendBack;
	done=false;

	return true;
}

/**********************************************************************
 Aktualizuje blender. Argumentem jest czas, ktory minal od
 poprzedniego wywolania fukcji update.
**********************************************************************/
void AnimationBlender::update(float delta)
{
	if(done) return;

	elapsedTime+=delta;

	if(elapsedTime>=duration)
	{
		elapsedTime=duration;

		a->weight=1.0f; b->weight=0.0f;	//Finalne przypisanie wag.

		if(blendBack)
		{
			if(a->finished)
			{
				elapsedTime=0;
				AnimationSet *tmpA=a, *tmpC=c;
				a=b=c=NULL;
				blend(tmpC,tmpA,NULL,duration,false);
				tmpA=tmpC=NULL;
			}
		}
		else
		{
			reset();
			return;
		}
	}

	a->weight=(elapsedTime/duration);
	b->weight=1-a->weight;
}
