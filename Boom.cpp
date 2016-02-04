
#include <SDL.h>
#include <sstream>

#include "Boom.h"

Boom::Boom()
{
	tex=0;
	bdir[1]=1.0f;
	color[0]=color[1]=color[2]=1;
	fade=0.8f; fadeStart=0; fadeDuration=1;
	width=height=10;
	life=15;
	active=false;
	z=1;
}

Boom::~Boom()
{
}

void Boom::update(float delta)
{
	if(!active) return;

	lifeTime+=delta;
	movementTime-=delta;
	z+=(delta)*2;

	life-=delta;	//Zmniejszamy zycie czasteczki.

	if(lifeTime>=fadeStart)
	{
		fade=0.8f-((lifeTime-fadeStart)/fadeDuration);
		if(fade<0) fade=0;
	}

	if(life<=0)
	{
		active=false;
		life=2;
		z=1;
		lifeTime=0;
	}
}

void Boom::draw()
{
	if(!active) return;

	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);

	Vec3 a(1,0,0), b((*camFront)[0],(*camFront)[1],(*camFront)[2]), c(0,1,0),d((*camFront));

	glTranslatef(b[0],b[1],b[2]);
	b.normalize();
	float angle=dot(a,b);
	angle=(acos(angle)*180.0f)/3.14159f;

	c.normalize();
	d.normalize();
	float angle2 = dot(c,d);
	angle2=(acos(angle2)*180.0f)/3.14159f;

	//a=(*camPos)-pos;
	//distance=a.length();

	if(b[2] < 0) angle=90+angle;
	else angle=90-angle;

	//if(c[0] < 0) angle2=90+angle2;
	//else angle2=90-angle2;

	glRotatef(angle,0,1,0);
	glRotatef(angle2-90,1,0,0);
	glScalef(z,z,z);
	//glRotatef(ang,0,0,1);

	glEnable( GL_BLEND );
	glDisable(GL_LIGHTING);
	glDisable( GL_LIGHT0 );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,tex);
	glColor4f(color[0],color[1],color[2],fade);
	glBegin(GL_QUADS);
		glTexCoord2f(1,0); glVertex3f(-width/2,-height/2,0);
		glTexCoord2f(1,1); glVertex3f(-width/2, height/2,0);
		glTexCoord2f(0,1); glVertex3f( width/2, height/2,0);
		glTexCoord2f(0,0); glVertex3f( width/2,-height/2,0);
	glEnd();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable( GL_BLEND );
    glEnable(GL_LIGHTING);
    glEnable( GL_LIGHT0 );

	if(life<=0)
	{
		pos[0]=pos[1]=pos[2]=0;
		life=2;
		fade=0.8f;
		z=1;
	}

	glPopMatrix();
}
