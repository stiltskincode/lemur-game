/*
 * structures.h
 *
 *  Created on: 2008-08-22
 *      Author: Admin
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_


struct Model
{
	StaticMesh mesh;
	int posX, posZ;
	float angle;
	int num;
	bool active;
};

struct Objects
{
	int num;
	StaticMesh mesh;
	string name;
	int barricade;
	int destructible;
};

#endif /* STRUCTURES_H_ */
