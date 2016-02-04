#pragma once

#define WRITE_ALL_LOGS

#include <string>
#include "Stage.h"
#include "Mathematics.h"
#include "LogWriter.h"

#define MAX_OBJECTS 100

class CollisionDetector
{
	struct BoundingCircle
	{
		Vec3 *position;
		float radius;
		float weight;
		std::string name;
		bool staticObject;
	};

	struct CollisionException
	{
		BoundingCircle *a, *b;
	};

	public:
		static void init();
		static void release();
		static void detect();
		static void setStage(Stage *s) {stage=s;}
		static bool add(float *f, std::string name="x",float radius=10.0f, float weight=1.0f, bool staticObject=false);
		//static bool add(Mesh *m, float radius=10.0f, float weight=1.0f, bool staticObject=false);
		static bool add(Vec3 *p, std::string name="x", float radius=10.0f, float weight=1.0f, bool staticObject=false);
		static bool remove(std::string name);

		static void stageCollision(BoundingCircle *bc);
		static void objectsCollision(BoundingCircle *dbc, BoundingCircle *sbc);

	private:
		static bool initialized;
		static Stage *stage;
		static BoundingCircle *objects[MAX_OBJECTS];
		static CollisionException *exceptions[MAX_OBJECTS];
		static unsigned int num;

		CollisionDetector();	//Konstruktor prywatny.
};
