#include "AnimationController.h"

#include <iostream>
using namespace std;

AnimationController::AnimationController()
{
	for(unsigned int i=0; i<MAX_ELEMENTS; i++) elements[i]=NULL;
	num=0;
	incr=NULL;
	value=0;
	numBlended=0;
	startWeight=0;
}

AnimationController::~AnimationController()
{
	for(unsigned int i=0; i<MAX_ELEMENTS; i++)
	{
		if(!elements[i]) continue;

		elements[i]->aset=NULL;
		delete elements[i];
		elements[i]=NULL;
	}
}

bool AnimationController::add(AnimationSet *a, std::string name, float maxWeight)
{
	for(unsigned int i=0; i<MAX_ELEMENTS; i++)
	{
		if(elements[i]) continue;

		elements[i]=new Element;
		elements[i]->aset=a;
		elements[i]->name=name;
		elements[i]->maxWeight=maxWeight;
		num++;

		return true;
	}
	return false;
}

void AnimationController::play(std::string name, float time, bool loop)
{
	numBlended=0;	//!!

	for(unsigned int i=0; i<MAX_ELEMENTS; i++)
	{
		if(!elements[i]) continue;
		if(elements[i]->name != name) continue;

		value=elements[i]->maxWeight - elements[i]->aset->weight;


		if(value<=0) value=0;
		else
		{
			incr=elements[i];
			blendDuration=time;
			elapsedTime=0;
			value=elements[i]->maxWeight - elements[i]->aset->weight;
			startWeight=elements[i]->aset->weight;

			//Obliczenie ilosci elementow, ktore beda braly udzial w blendingu.
			//Ilosc elementow jest mniejsza o 1, gdyz jeden element (incr) ma zwiekszana wage.
			for(unsigned int j=0; j<MAX_ELEMENTS; j++)
			{
				//Pomijamy elementy puste i element, ktorego waga bedzie zwiekszana.
				if(!elements[j] || elements[j]==incr) continue;

				//Zwiekszamy ilosc elementow, jezeli posiadaja one wage wieksza od 0.
				if(elements[j]->aset->weight>0)	numBlended++;
			}

			break;
		}
	}
}

void AnimationController::update(float delta)
{
	if(incr)
	{
		elapsedTime+=delta;

		//Nowa waga dla elementu 'zwiekszanego'.
		float v=startWeight+value*(elapsedTime/blendDuration);
		if(v>incr->maxWeight) v=incr->maxWeight;

		//Obliczenie przyrostu wagi.
		float increase=(v-incr->aset->weight);
		incr->aset->weight+=increase;

		if(numBlended>0)	//Jezeli istnieja elementy do blendingu.
		{
			increase/=numBlended;

			//Dla kazdego elementu, zmniejszamy jego wage o dodany do incr przyrost
			//podzielony przez numBlended.
			for(unsigned int i=0; i<MAX_ELEMENTS; i++)
			{
				if(!elements[i] || elements[i]==incr || elements[i]->aset->weight<=0) continue;

				elements[i]->aset->weight-=increase;
			}

			bool loop=true;

			while(loop && numBlended>0)
			{
				loop=false;

				//Sprawdzamy, czy po zmmniejszeniu wagi, ktorykolwiek z elementow
				//posiada wage < 0. Jezeli tak, zostaje wykluczony z blendingu.
				for(unsigned int i=0; i<MAX_ELEMENTS; i++)
				{
					if(!elements[i] || elements[i]==incr) continue;
					if(elements[i]->aset->weight>=0) continue;

					loop=true;

					numBlended--;	//Element zostaje wykluczony.

					//Obliczenie nadmiaru wagi, jaki zostal odjety od wykluczonego elementu.
					float x=(elements[i]->aset->weight*-1)/numBlended;

					elements[i]->aset->weight=0;

					if(numBlended<=0) break;

					//Teraz nalezy 'rozdysponowac' nadmiar wagi, jaki zostal odjety od wykluczonego
					//elementu. Po procesiewykluczenia elementy, nalezy ponownie uruchomic petle (i=0);
					for(unsigned int j= 0; j<MAX_ELEMENTS; j++)
					{
						if(!elements[j] || elements[j]==incr || elements[j]==elements[i]) continue;
						elements[j]->aset->weight-=x;
					}
				}
			}
		}

		if(elapsedTime>=blendDuration)
		{
			elapsedTime=blendDuration=0;
			incr=NULL;
		}
	}
}
