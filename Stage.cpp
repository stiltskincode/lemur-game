#include "Stage.h"

#include <iostream>
#include <fstream>

using namespace std;

string buffer;

void Stage::loadExternResources(ifstream &file, deque<ExternResource> &res)
{
	file>>buffer;
	while(buffer!="-----")
	{
		res.push_back(ExternResource());

		res.back().name=buffer;
		file>>res.back().z>>res.back().x>>res.back().angle>>buffer;
	}
}

void Stage::loadResources(std::ifstream &file)
{
	string location;
	file>>location>>buffer;

	while(buffer!="-----")
	{
		resources.push_back(StageResource());
		resources.back().mesh.load(location+"/"+buffer);
		file>>resources.back().wall>>buffer;
	}

}

void Stage::loadStage(ifstream &file)
{
	int width, height, id, angle;

	//Pobranie wymiarow planszy.
	file>>height>>width;

	for(int h=0; h<height; h++)
	{
		stage.push_back(deque<StageObject>());
		for(int w=0; w<width; w++)
		{
			stage[h].push_back(StageObject());

			file>>id>>angle;
			stage[h].back().mesh = &resources[id].mesh;
			stage[h].back().wall = resources[id].wall;
			stage[h].back().angle = angle*90;
		}
	}
}

void Stage::load(string fileName)
{
	size=200.0f;
	startAngle=startDistance=0;

	ifstream file(fileName.c_str());

	while(!file.eof())
	{
		file>>buffer;
		if(buffer=="#stage_resources") loadResources(file);
		if(buffer=="#stage") loadStage(file);
		if(buffer=="#monsters") loadExternResources(file, monsters);
		if(buffer=="#blockades") loadExternResources(file, blockades);
		if(buffer=="#start") file>>startPos[2]>>startPos[0]>>startAngle>>startDistance;
	}

	//file.close(); file.clear();
}

void Stage::draw()
{
	glColor3f(1,1,1);
	glPushMatrix();
		glTranslatef(size/2.0f,0,size/2.0f);
		for(unsigned int h=0; h<stage.size(); h++)
		{
			glPushMatrix();
				for(unsigned int w=0; w<stage[h].size(); w++)
				{
					glPushMatrix();
						glRotatef(stage[h][w].angle,0,1,0);
						stage[h][w].mesh->draw();
					glPopMatrix();
					glTranslatef(size,0.0f,0.0f);
				}
			glPopMatrix();
			glTranslatef(0,0,size);
		}
	glPopMatrix();
}

