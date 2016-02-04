#ifndef STAGE_H_
#define STAGE_H_

#include "Mesh.h"
#include <string>
#include <deque>

class Stage
{
	public:

	class ExternResource
	{
		public:
			std::string name;
			float angle;
			int x,z;
	};

	class StageResource
	{
		public:
			Mesh mesh;
			bool wall;
	};

	class StageObject
	{
		public:
			StageObject() {reset();}
			~StageObject() {reset();}
			void reset()
			{
				wall=false;
				angle=0;
				mesh=NULL;
				monster=bomb=blockade=NULL;
			}

			bool wall;
			float angle;		//Kat obrotu obiektu.
			Mesh *mesh;

			Object3D *monster;	//Potwor, ktory znajduje sie na polu.
			Object3D *bomb;		//Bomba, ktora znajduje sie na polu.
			Object3D *blockade;	//Element zniszczalny, stojacy na polu.
	};

	public:
		void clear() { resources.clear(); stage.clear(); monsters.clear(); blockades.clear();}
		void load(std::string fileName);
		void draw();

	//private:
		std::deque<StageResource> resources;
		std::deque< deque<StageObject> > stage;
		std::deque<ExternResource> monsters, blockades;
		float size;

		float startAngle, startDistance;
		Vec3 startPos;

		void loadResources(std::ifstream &file);
		void loadStage(std::ifstream &file);
		void loadExternResources(std::ifstream &file, std::deque<ExternResource> &res);
};

#endif
