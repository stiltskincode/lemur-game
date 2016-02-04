#include "Monster.h"

using namespace std;

Monster::Monster()
{
	stage=NULL;

	speed=150;

	mesh.getFront().xyz(0,0,-1);
	front.xyz(0,0,-1);

	//Wygenerowanie 'wirtualnej' konfiguracji kluczy.
	for(int i=0; i<11; i++) keys[i]=SDLKey(i);

	for(int i=0; i<11; i++) newkeystate[i]=0;

	stageX=0; stageZ=0;

	newkeystate[moveForward]=1;

	keystate = newkeystate;

	sumRotationAngle=0;
	reverse=false;
	step=0;

	correctMatrix=30.0f;
}

Monster::~Monster()
{
	stage=NULL;
}

void Monster::setPosition(float x, float y, float z)
{
	stageX=int(x/200);
	stageZ=int(z/200);

	mesh.tmat[12]=x; mesh.tmat[13]=y; mesh.tmat[14]=z;
	mesh.setPosition(x,y,z);

	stage->stage[stageZ][stageX].monster = this;
}

void Monster::rotateRight()
{
	rotationAngle=-step*90.0f/157.079633f;
	//rotationAngle=-30;

	sumRotationAngle+=rotationAngle;

	if(sumRotationAngle <= -90 )
	{
		rotationAngle=sumRotationAngle+90;
		sumRotationAngle=0;
		keystate[turnRight]=0;
	}

	Mat4 rot; rot.rotationY(rotationAngle);
	mesh.tmat*=rot;
	mesh.getFront().rotate(rotationAngle,Vec3(0,1,0));
	mesh.getFront().normalize();
}

void Monster::rotateLeft()
{
	rotationAngle=step*90.0f/157.079633f;
	//rotationAngle=30;

	sumRotationAngle+=rotationAngle;

	if(reverse)
	{
		if(sumRotationAngle >= 180)
		{
			rotationAngle=sumRotationAngle-180;
			sumRotationAngle=0;
			keystate[turnLeft]=0;
			keystate[moveForward]=1;

			reverse=false;
		}
	}
	else
	{
		if(sumRotationAngle >= 90)
		{
			rotationAngle=sumRotationAngle-90;
			sumRotationAngle=0;
			keystate[turnLeft]=0;
		}
	}

	Mat4 rot; rot.rotationY(rotationAngle);
	mesh.tmat*=rot;
	mesh.getFront().rotate(rotationAngle,Vec3(0,1,0));
	mesh.getFront().normalize();
}

void Monster::handleInput()
{
	//keystate = SDL_GetKeyState(NULL);
	keystate = newkeystate;

	//DUPA DUPA
	//mesh.setFront(front);
	mesh.tmat[12] = mesh.getPosition()[0];
	mesh.tmat[13] = mesh.getPosition()[1];
	mesh.tmat[14] = mesh.getPosition()[2];
	//position = mesh.getPosition();

	step=(*delta)*speed;

	if(reverse==true) step*=2;

	int area[4]={0,0,0,0};
	int x,z;

	x=int(mesh.getPosition()[0]/200);
	z=int(mesh.getPosition()[2]/200);

	Vec3 &pos = mesh.getPosition();
	Vec3 &front = mesh.getFront();

	//Korekcja wektora front.
	if(!keystate[turnRight] && !keystate[turnLeft])
	{
		if(abs(front[0]) > abs(front[2])) { front[2]=0; front[0]=(front[0]>0)? 1:-1;}
		if(abs(front[0]) < abs(front[2])) { front[0]=0; front[2]=(front[2]>0)? 1:-1;}
	}

	//Korekcja pozycji.
	if(!keystate[turnRight] && !keystate[turnLeft] && keystate[moveForward])
	{
		if(front[0]==0) pos[0]=x*200+100;
		else pos[2]=z*200+100;
	}

	//Korekcja macierzy.
	correctMatrix-=(*delta);
	if(!keystate[turnRight] && !keystate[turnLeft] && keystate[moveForward] && correctMatrix<=0)
	{
		Mat3 m;

		if(front[0]==-1) m.rotationY(-90);
		else if(front[0]==1) m.rotationY(90);
		else if(front[2]==1) m.rotationY(180);

		mesh.tmat=m;

		correctMatrix=30.0f;
	}


	//Indeksy elementow otaczajacych potwora.
	int ax[4]={0,0,0,0}, az[4]={0,0,0,0};
	Vec3 vec(0,0,-1);

	//Przygotowanie indeksow elementow planszy, ktore otaczaja potwora.
	if(front[2]==-1) { ax[0]=x-1; ax[1]=x;   ax[2]=x+1;  az[0]=z;   az[1]=z-1; az[2]=z;  }
	if(front[2]==1 ) { ax[0]=x+1; ax[1]=x;   ax[2]=x-1;  az[0]=z;   az[1]=z+1; az[2]=z;	 }
	if(front[0]==-1) { ax[0]=x;   ax[1]=x-1; ax[2]=x;    az[0]=z+1; az[1]=z;   az[2]=z-1;}
	if(front[0]==1)  { ax[0]=x;   ax[1]=x+1; ax[2]=x;    az[0]=z-1; az[1]=z;   az[2]=z+1;}
	ax[3]=x; az[3]=z;

	for(int i=0; i<4; i++)
	{
		area[i] = 0;
		if(stage->stage[az[i]][ax[i]].wall) continue;
		if(stage->stage[az[i]][ax[i]].monster)
		{
			if(stage->stage[az[i]][ax[i]].monster != (void*)this) continue;
		}
		if(stage->stage[az[i]][ax[i]].bomb) continue;
		area[i] = 1;
	}

	//for(int i=0; i<4; i++) cout<<area[i]<<" "; cout<<endl;

	if(stageX != x || stageZ != z) //Biezaca akcja zakonczyla sie.
	{
		int dir=-1;

		if(area[3])	//Jezeli pole, na ktorym sie znajdujemy jest wolne
		{
			srand(SDL_GetTicks());

			if( area[0] &&  area[1] &&  area[2]) dir=rand()%3;				//1 1 1
			if( area[0] &&  area[1] && !area[2]) dir=rand()%2;				//1 1 0
			if( area[0] && !area[1] && !area[2]) dir=0;						//1 0 0
			if(!area[0] &&  area[1] &&  area[2]) dir=(rand()%2)+1;			//0 1 1
			if(!area[0] && !area[1] &&  area[2]) dir=2;						//0 0 1
			if(!area[0] &&  area[1] && !area[2]) dir=1;						//0 1 0
			if( area[0] && !area[1] &&  area[2]) dir=((rand()%2)==0)? 0:2;	//1 0 1
			if(!area[0] && !area[1] && !area[2]) reverse=true;				//0 0 0

			stage->stage[stageZ][stageX].monster = NULL;
			stage->stage[z][x].monster = this;
		}
		else
		{
			dir=-1; reverse=true;
		}

		stageX=x; stageZ=z;

		for(int i=0; i<11; i++) keystate[i]=0;
		switch(dir)
		{
			case 0:
				keystate[moveForward]=1;
				keystate[turnLeft]=1;
			break;

			case 1:
				keystate[moveForward]=1;
			break;

			case 2:
				keystate[moveForward]=1;
				keystate[turnRight]=1;
			break;

			default:
				keystate[turnLeft]=1;
			break;
		}
	}
}

void Monster::init(Stage *stage)
{
	this->stage = stage;
}

void Monster::load(std::string location)
{
	mesh.load(location+"/mesh.dim", location+"/textures");
	mesh.loadSkinningShader(location+"/vertex.vs");
	mesh.loadSkeleton(location+"/skeleton.dim");
	mesh.skeleton->update();
	mesh.skeleton->updateAttributeMatrices();

	if(anims[stand].load(location+"/animations/stand.dim"))
	{
		anims[stand].bind(mesh.skeleton);
		anims[stand].weight=1;
	}
	if(anims[moveForward].load(location+"/animations/moveForward.dim"))
	{
		anims[moveForward].bind(mesh.skeleton);
	}
	if(anims[turnLeft].load(location+"/animations/turnLeft.dim"))
	{
		anims[turnLeft].bind(mesh.skeleton);
	}
	if(anims[die].load(location+"/animations/die.dim"))
	{
		anims[die].bind(mesh.skeleton);
		anims[die].stop();
		anims[die].setLoop(false);
	}

	a=&anims[stand];
	b=&anims[moveForward];
}
