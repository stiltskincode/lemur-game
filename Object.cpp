#include "Object.h"

Object::Object()
{
	id=0;
	name="no_name_object";
	active=true;
}

Object::~Object()
{
	id=0;
}

void Object::setName(std::string name)
{
	this->name = name;
}

void Object::setID(unsigned int id)
{
	this->id = id;
}

void Object::setActive(bool value)
{
	active=value;
}

bool Object::isActive()
{
	return active;
}

std::string& Object::getName()
{
	return name;
}

unsigned int& Object::getID()
{
	return id;
}
