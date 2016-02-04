#include "ExplosionElement.h"

float *ExplosionElement::delta=NULL;

void ExplosionElement::init(float *d)
{
	delta=d;
}

ExplosionElement::ExplosionElement()
{
	mesh=NULL;
	created=active=false;
	duration=fullSizeDuration=elapsedTime=0.0f;
}

ExplosionElement::~ExplosionElement()
{
	mesh=NULL;
}

void ExplosionElement::create(Mesh *mesh)
{
	this->mesh = mesh;
	created = true;
}

void ExplosionElement::explode(Vec3 position, float duration, float fullSizeDuration)
{
	if(created && !active) active = true;
	else return;

	this->position = position;
	this->duration = duration;
	this->fullSizeDuration = fullSizeDuration;
}

void ExplosionElement::draw()
{
	if(!active) return;

	elapsedTime+=(*delta);

	if(elapsedTime>=duration/2.0f && fullSizeDuration>0.0f)
	{
		elapsedTime=duration/2.0f;
		fullSizeDuration-=(*delta);
	}
	else if(elapsedTime>=duration)
	{
		duration=fullSizeDuration=elapsedTime=0.0f;
		active=false;
		return;
	}

	float value=sin(3.1415963f*(elapsedTime/duration));
	float value2=1;
	if(elapsedTime >= duration/2 )
	{
		//value2=1-((elapsedTime-(duration/2))*2);
		//value2=1-value;
		value2=1-((elapsedTime-(duration/2))/(duration/2));
	}

	//value2/=2;

	glPushMatrix();
		glEnable( GL_BLEND );
		//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		glDepthMask(GL_FALSE);
		glTranslatef(position[0],position[1],position[2]);
		glScalef(value,value,value);
		//glColor3f(1,1,1);
		glColor4f(value2,value2,value2,value);
		//glColor4f(0.1,0.1,0.1,1);
		mesh->draw();
		glDisable( GL_BLEND );
		glDepthMask(GL_TRUE);
	glPopMatrix();
}
