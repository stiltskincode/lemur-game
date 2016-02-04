//#include "Object3D.h"
#include "Camera.h"
//#include "Object.h"

Camera::Camera()
{
	front.xyz(1,0,0);
	up.xyz(0,1,0);
	right.xyz(0,0,1);
	position.xyz(0,0,0);
	speed=1;
	active=true;
}

Camera::~Camera() {}

void Camera::yaw(float angle)
{
	if(!active) return;

	if(!angle) return;
	front.rotate(angle,up); front.normalize();
	right=cross(front,up);
	right.normalize();
}

void Camera::pitch(float angle)
{
	if(!active) return;

	if(!angle) return;
	front.rotate(angle,right);
	front.normalize();
	up=cross(right,front);
	up.normalize();
}

void Camera::roll(float angle)
{
	if(!active) return;

	if(!angle) return;
	up.rotate(angle,front);
	up.normalize();
	right=cross(front,up);
	right.normalize();
}

void Camera::move(float f, float u, float r)
{
	if(!active) return;

	Vec3 tmp;
	if(f) { tmp=front*f; position+=tmp; }
	if(u) { tmp=up*u; position+=tmp; }
	if(r) { tmp=right*r; position+=tmp; }
}

void Camera::update()
{
	if(!active) return;

	gluLookAt( position.v[0],position.v[1],position.v[2],
			   position.v[0]+front.v[0],
			   position.v[1]+front.v[1],
			   position.v[2]+front.v[2],
			   up.v[0],up.v[1],up.v[2] );
}

void Camera::useSDLEvents()
{
	if(EventManager::keyPressed(SDLK_w)) move((*delta)*speed,0,0);
	if(EventManager::keyPressed(SDLK_s)) move((*delta)*-speed,0,0);
	if(EventManager::keyPressed(SDLK_a)) move(0,0,(*delta)*-speed);
	if(EventManager::keyPressed(SDLK_d)) move(0,0,(*delta)*speed);
	if(EventManager::keyPressed(SDLK_PAGEUP)) move(0,(*delta)*speed,0);
	if(EventManager::keyPressed(SDLK_PAGEDOWN)) move(0,(*delta)*-speed,0);
	if(EventManager::keyDown(SDLK_KP_PLUS)) speed+=100;
	if(EventManager::keyDown(SDLK_KP_MINUS)) speed-=100;

	if(EventManager::mouseButtonDown(3))
	{
		yaw(-float(EventManager::mouseRelX)/10.0f);
		pitch(-float(EventManager::mouseRelY)/10.0f);
	}
}

void Camera::draw()
{
	/*
	Vec3 a,b,c,d,a2,b2,c2,d2,vup,vright,vfront;
	vup=front|right; vup.normalize();

	a=position-right-vup;
	b=position+right-vup;
	c=position+right+vup;
	d=position-right+vup;

	glColor3f(1,0,0);
	//glLineWidth(5);
	glBegin(GL_LINE_LOOP);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
		glVertex3f(d[0],d[1],d[2]);
		glVertex3f(a[0],a[1],a[2]);
	glEnd();

	a2=a+front*(-2);
	b2=b+front*(-2);
	c2=c+front*(-2);
	d2=d+front*(-2);

	glBegin(GL_LINE_LOOP);
		glVertex3f(a2[0],a2[1],a2[2]);
		glVertex3f(b2[0],b2[1],b2[2]);
		glVertex3f(c2[0],c2[1],c2[2]);
		glVertex3f(d2[0],d2[1],d2[2]);
		glVertex3f(a2[0],a2[1],a2[2]);
	glEnd();


	glBegin(GL_LINES);
		glVertex3f(a[0],a[1],a[2]); glVertex3f(a2[0],a2[1],a2[2]);
		glVertex3f(b[0],b[1],b[2]); glVertex3f(b2[0],b2[1],b2[2]);
		glVertex3f(c[0],c[1],c[2]); glVertex3f(c2[0],c2[1],c2[2]);
		glVertex3f(d[0],d[1],d[2]); glVertex3f(d2[0],d2[1],d2[2]);
	glEnd();

	a+=front*(2);
	b+=front*(2);
	c+=front*(2);
	d+=front*(2);
	glBegin(GL_LINE_LOOP);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
		glVertex3f(d[0],d[1],d[2]);
		glVertex3f(a[0],a[1],a[2]);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(a[0],a[1],a[2]); glVertex3f(position[0],position[1],position[2]);
		glVertex3f(b[0],b[1],b[2]); glVertex3f(position[0],position[1],position[2]);
		glVertex3f(c[0],c[1],c[2]); glVertex3f(position[0],position[1],position[2]);
		glVertex3f(d[0],d[1],d[2]); glVertex3f(position[0],position[1],position[2]);
	glEnd();
	*/
}
