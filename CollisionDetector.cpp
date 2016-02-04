#include "CollisionDetector.h"

#include <iostream>

using namespace std;

unsigned int CollisionDetector::num=0;
bool CollisionDetector::initialized=false;
Stage *CollisionDetector::stage=NULL;
CollisionDetector::BoundingCircle *CollisionDetector::objects[MAX_OBJECTS];
CollisionDetector::CollisionException *CollisionDetector::exceptions[MAX_OBJECTS];

/**********************************************************************
 Inicjalizuje detektor.
**********************************************************************/
void CollisionDetector::init()
{
	for(int i=0; i<MAX_OBJECTS; i++) { objects[i]=NULL; exceptions[i]=NULL; }
	initialized=true;
}

/**********************************************************************
 Zwalnia zasoby zajete przez detektora kolizji.
**********************************************************************/
void CollisionDetector::release()
{
	for(int i=0; i<MAX_OBJECTS; i++)
	{
		if(objects[i])
		{
			delete objects[i]->position; objects[i]->position=NULL;
			delete objects[i]; objects[i]=NULL;
		}
		if(exceptions[i])
		{
			exceptions[i]->a=exceptions[i]->b=NULL;
			delete exceptions[i]; exceptions[i]=NULL;
		}
	}
	stage=NULL;
}

/**********************************************************************
 Dodaje obiekt do detektora kolizji.
**********************************************************************/
bool CollisionDetector::add(float *f, string name, float radius, float weight, bool staticObject)
{
	if(!initialized)	//Jezeli klasa nie zostala zainicjalizowana.
	{
		writeErr("Nie mozna dodac nowego obiektu. Klasa CollisionDetector nie zostala zainicjalizowana. "
				 "W celu inicjalizacji, nalezy wywolac metode: CollisionDetector::init().");
		return false;
	}

	if(!f)	//Jezeli podano pusty wskaznik.
	{
		writeErr("Nie mozna dodac nowego obiektu. Przekazany wskaznik jest pusty!");
		return false;
	}

	if(num>=MAX_OBJECTS)
	{
		writeErr("Nie mozna dodac nowego obiektu. Przekroczono maksymalna ilosc obiektow!");
		return false;
	}

	int index=0;

	for(int i=0; i<MAX_OBJECTS; i++)
	{
		if(objects[i]) continue;

		objects[i]=new BoundingCircle;
		objects[i]->position = new Vec3(f);
		objects[i]->radius = radius;
		objects[i]->weight = weight;
		objects[i]->name = name;
		objects[i]->staticObject=staticObject;
		num++;
		index=i;

		break;
	}

	for(int i=0; i<MAX_OBJECTS; i++)
	{
		if(index==i || !objects[i]) continue;

		float diff = ((*objects[i]->position) - (*objects[index]->position)).length();
		if( diff < (objects[i]->radius + objects[index]->radius) )
		{
			for(int j=0; j<MAX_OBJECTS; j++)
			{
				if(exceptions[j]) continue;

				exceptions[j]=new CollisionException;
				exceptions[j]->a=objects[index];
				exceptions[j]->b=objects[i];
			}
		}

		//if(objects[i]->position)
	}

	return true;
}

/**********************************************************************
 Dodaje obiekt do detektora kolizji.
**********************************************************************/
bool CollisionDetector::add(Vec3 *p, string name, float radius, float weight, bool staticObject)
{
	return add(p->v, name, radius, weight, staticObject);
}

/**********************************************************************
 Usuwa obiekt z detektora kolizji.
**********************************************************************/
bool CollisionDetector::remove(std::string name)
{
	for(unsigned int i=0; i<MAX_OBJECTS; i++)
	{
		if(objects[i] && objects[i]->name==name)
		{
			for(int j=0; j<MAX_OBJECTS; j++)
			{
				if(!exceptions[j]) continue;

				if(exceptions[j]->a==objects[i] || exceptions[j]->b==objects[i])
				{
					exceptions[j]->a=exceptions[j]->b=NULL;
					delete exceptions[j]; exceptions[j]=NULL;
				}
			}
			delete objects[i]->position; objects[i]->position=NULL;
			delete objects[i]; objects[i]=NULL;
			num--;

			return true;
		}
	}

	return false;
}

/**********************************************************************
 Glowna metoda sprawdzajaca kolizje na planszy.
**********************************************************************/
void CollisionDetector::detect()
{
	if(stage)
	{
		for(unsigned int i=0; i<MAX_OBJECTS; i++)
		{
			if(!objects[i]) continue;
			stageCollision(objects[i]);
		}
	}

	for(unsigned int i=0; i<MAX_OBJECTS; i++)
	{
		if(!objects[i]) continue;
		for(unsigned int j=i; j<MAX_OBJECTS; j++)
		{
			if(!objects[j]) continue;
			objectsCollision(objects[i],objects[j]);
		}
	}

	for(int i=0; i<MAX_OBJECTS; i++)
	{
		if(!exceptions[i]) continue;

		float diff = ((*exceptions[i]->a->position) - (*exceptions[i]->b->position)).length();
		if(diff >= exceptions[i]->a->radius + exceptions[i]->b->radius )
		{
			exceptions[i]->a=exceptions[i]->b=NULL;
			delete exceptions[i];
			exceptions[i]=NULL;
		}
	}
}

/**********************************************************************
 Sprawdza kolizje obiektu BoundingCircle z plansza.
**********************************************************************/
void CollisionDetector::stageCollision(BoundingCircle *bc)
{
	static int area[3][3];
	static int x,z;
	static float radius;

	Vec3 &pos = *bc->position;	//Uproszczenie zapisu.
	radius = bc->radius;		//Uproszczenie zapisu.

	x=int(pos[0]/200);
	z=int(pos[2]/200);

	area[0][0] = stage->stage[z-1][x-1].wall;
	area[0][1] = stage->stage[z-1][x  ].wall;
	area[0][2] = stage->stage[z-1][x+1].wall;
	area[1][0] = stage->stage[z  ][x-1].wall;
	area[1][1] = stage->stage[z  ][x  ].wall;
	area[1][2] = stage->stage[z  ][x+1].wall;
	area[2][0] = stage->stage[z+1][x-1].wall;
	area[2][1] = stage->stage[z+1][x  ].wall;
	area[2][2] = stage->stage[z+1][x+1].wall;

	if(area[0][1]) if(pos[2]-(z*200) < radius) pos[2]=(z*200)+radius;
	if(area[2][1]) if((z*200+200)-pos[2] < radius) pos[2]=(z*200+200)-radius;
	if(area[1][0]) if(pos[0]-(x*200) < radius) pos[0]=(x*200)+radius;
	if(area[1][2]) if((x*200+200)-pos[0] < radius) pos[0]=(x*200+200)-radius;

	if(area[0][0])
	{
		Vec3 corner((x*200),0,(z*200));
		Vec3 distance = pos-corner;
		if(distance.length()<radius)
		{
			distance.normalize();
			distance*=radius;
			pos=corner+distance;
		}
	}
	if(area[0][2])
	{
		Vec3 corner((x*200+200),0,(z*200));
		Vec3 distance = pos-corner;
		if(distance.length()<radius)
		{
			distance.normalize();
			distance*=radius;
			pos=corner+distance;
		}
	}
	if(area[2][0])
	{
		Vec3 corner((x*200),0,(z*200+200));
		Vec3 distance = pos-corner;
		if(distance.length()<radius)
		{
			distance.normalize();
			distance*=radius;
			pos=corner+distance;
		}
	}
	if(area[2][2])
	{
		Vec3 corner((x*200+200),0,(z*200+200));
		Vec3 distance = pos-corner;
		if(distance.length()<radius)
		{
			distance.normalize();
			distance*=radius;
			pos=corner+distance;
		}
	}
}

void CollisionDetector::objectsCollision(BoundingCircle *a, BoundingCircle *b)
{
	bool xxx = false;

	for(int i=0; i<MAX_OBJECTS; i++)
	{
		if(!exceptions[i]) continue;

		if( exceptions[i]->a==a && exceptions[i]->b==b ) {xxx=true; break;}
		if( exceptions[i]->a==b && exceptions[i]->b==a ) {xxx=true; break;}
	}

	if(xxx) return;

	Vec3 diff = (*a->position) - (*b->position);
	float dist = diff.length();

	if(dist < a->radius+b->radius)
	{
		diff.normalize();
		diff*=a->radius+b->radius;
		(*a->position)=(*b->position)+diff;
	}
}
