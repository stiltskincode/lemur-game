#include "LemurCamera.h"

LemurCamera::LemurCamera()
{
	characterFront=characterPosition=NULL;
	rotationAngle=NULL;
	angleLeft=0;
	distVar=0.0f;
	distVarLimit=350.0f;
}

LemurCamera::~LemurCamera()
{
	characterFront=characterPosition=NULL;
	rotationAngle=NULL;
}

void LemurCamera::init(Vec3 *characterFront, Vec3 *characterPosition, float *rotationAngle)
{
	active=true;
	this->characterFront=characterFront;
	this->characterPosition=characterPosition;
	this->rotationAngle=rotationAngle;

	front=(*characterFront);
}

void LemurCamera::update()
{
	if(!active) return;

	angleLeft+=(*rotationAngle);
	if(angleLeft>180) angleLeft-=360;
	if(angleLeft<-180) angleLeft+=360;

	if(angleLeft>1.0f || angleLeft<-1.0f)
	{
		float angle = (*delta)*60.0f*(angleLeft/30.0f);
		front.rotate(0,angle,0);
		angleLeft-=angle;
	}

	else
	{
		float angle = acos(dot(front,(*characterFront)))*180.0f/3.14159f;

		if(angle>2)
		{
			if(angleLeft<0) angle*=-1;
			angleLeft+=angle;
		}
	}

	if((*characterPosition) != lastCharacterPosition)
	{
		distVar+=(*delta)*100;
		if(distVar>distVarLimit) distVar=distVarLimit;
		lastCharacterPosition=(*characterPosition);
	}
	else
	{
		distVar-=(*delta)*300;
		if(distVar<0) distVar=0;
	}

	//Vec3 f=front*-(250+distVar);
	//position=(*characterPosition)+f;
	//position[1]=350+(distVar/2.0f);

	 Vec3 f=front*-(500+distVar);
	 position=(*characterPosition)+f;
	 position[1]=500+(distVar/2.0f);

	gluLookAt(position.v[0],position.v[1],position.v[2],
			 (*characterPosition).v[0], 150, (*characterPosition).v[2],
			  0,1,0 );
}
