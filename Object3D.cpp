#include "Object3D.h"

float *Object3D::delta = NULL;
bool Object3D::initialized = false;

void Object3D::init(float *delta)
{
	Object3D::delta = delta;
	initialized = true;
}

void Object3D::release()
{
	delta = NULL;
}

Object3D::Object3D()
{
	sharedElement = false;
}

Object3D::~Object3D()
{

}

void Object3D::setFront(Vec3& f)
{
	front=f;
}

void Object3D::setUp(Vec3& u)
{
	up=u;
}

void Object3D::setRight(Vec3& r)
{
	right=r;
}

void Object3D::setPosition(Vec3& p)
{
	position=p;
}

Vec3& Object3D::getFront()
{
	return front;
}

Vec3& Object3D::getUp()
{
	return up;
}

Vec3& Object3D::getRight()
{
	return right;
}

Vec3& Object3D::getPosition()
{
	return position;
}
