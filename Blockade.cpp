#include "Blockade.h"

Blockade::Blockade()
{
	newkeystate[moveForward]=0;
	newkeystate[stand]=1;
}

Blockade::~Blockade()
{
}

void Blockade::setPosition(float x, float y, float z)
{
	stageX=int(x/200);
	stageZ=int(z/200);

	mesh.tmat[12]=x; mesh.tmat[13]=y; mesh.tmat[14]=z;
	mesh.setPosition(x,y,z);

	stage->stage[stageZ][stageX].blockade = this;
	stage->stage[stageZ][stageX].wall = true;
}

void Blockade::load(std::string location)
{
	cout<<mesh.load(location+"/mesh.dim", location+"/textures")<<endl;
	mesh.loadSkinningShader(location+"/vertex.vs");
	mesh.loadSkeleton(location+"/skeleton.dim");
	mesh.skeleton->update();
	mesh.skeleton->updateAttributeMatrices();

	if(anims[stand].load(location+"/animations/stand.dim"))
	{
		anims[stand].bind(mesh.skeleton);
		anims[stand].weight=1;
	}
	if(anims[die].load(location+"/animations/die.dim"))
	{
		anims[die].bind(mesh.skeleton);
		anims[die].stop();
		anims[die].setLoop(false);
	}

	a=&anims[stand];
	b=&anims[die];
}

void Blockade::handleInput()
{
	keystate = newkeystate;

	mesh.tmat[12] = mesh.getPosition()[0];
	mesh.tmat[13] = mesh.getPosition()[1];
	mesh.tmat[14] = mesh.getPosition()[2];

}
