#include "Character.h"

#include <iostream>
#include <algorithm>

using namespace std;

Character::Character()
{
	speed=300.0f;
	initialized=actionLasts=initAction=false;
	a=b=actionA=actionB=NULL;
	rotationAngle=0;
	keystate=NULL;
	mouseLeft = mouseRight = false;
	resetMouse=0.1f;
	rotationSpeed = 1;
	alive=true;
	dying=false;

	mesh.getFront().xyz(0,0,-1);
	front = mesh.getFront();

	forceAction = forceWalkForward = false;

	bombsLeft=bombsLimit=10;
	for(int i=0; i<MAX_CHARACTER_BOMBS; i++) bombs[i]=NULL;

	suit = NULL;
}

Character::~Character()
{
	a=b=actionA=actionB=NULL;
	keystate=NULL;

	for(int i=0; i<MAX_CHARACTER_BOMBS; i++) if(bombs[i]) {delete bombs[i]; bombs[i]=NULL;}

	if(suit) delete suit; suit=NULL;
}

void Character::load(string location)
{
	mesh.load(location+"/mesh.dim");
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
	if(anims[moveBackward].load(location+"/animations/moveBackward.dim"))
	{
		anims[moveBackward].bind(mesh.skeleton);
	}
	if(anims[putBomb].load(location+"/animations/putBomb.dim"))
	{
		anims[putBomb].bind(mesh.skeleton);
		anims[putBomb].stop();
		anims[putBomb].setLoop(false);
	}
	if(anims[turnLeft].load(location+"/animations/turnLeft.dim"))
	{
		anims[turnLeft].bind(mesh.skeleton);
	}
	if(anims[turnRight].load(location+"/animations/turnRight.dim"))
	{
		anims[turnRight].bind(mesh.skeleton);
	}
	if(anims[strafeLeft].load(location+"/animations/strafeLeft.dim"))
	{
		anims[strafeLeft].bind(mesh.skeleton);
	}
	if(anims[strafeRight].load(location+"/animations/strafeRight.dim"))
	{
		anims[strafeRight].bind(mesh.skeleton);
	}
	if(anims[die].load(location+"/animations/die.dim"))
	{
		anims[die].bind(mesh.skeleton);
		anims[die].stop();
		anims[die].setLoop(false);
	}

	a=&anims[stand];
	b=&anims[moveForward];

	readKeyConfig(location+"/keys.txt");	//Odczytanie konfiguracji klawiszy.
}

void Character::handleInput()
{
	keystate = SDL_GetKeyState(NULL);

	if(forceAction)
	{
		if(forceWalkForward) keystate[keys[moveForward]]=1;
		else
		{
			forceAction=false;
			keystate[keys[moveForward]]=0;
		}
	}
}

void Character::rotateRight()
{
	if(forceAction) return;
	rotationAngle=(-(*delta)*360.0f/2.0f)*rotationSpeed;
	Mat4 rot; rot.rotationY(rotationAngle);
	mesh.tmat*=rot;
	mesh.getFront().rotate(rotationAngle,Vec3(0,1,0));
}

void Character::rotateLeft()
{
	if(forceAction) return;
	rotationAngle=((*delta)*360.0f/2.0f)*rotationSpeed;
	Mat4 rot; rot.rotationY(rotationAngle);
	mesh.tmat*=rot;
	mesh.getFront().rotate(rotationAngle,Vec3(0,1,0));
}

void Character::walkForward()
{
	Vec3 front=mesh.getFront();
	front*=((*delta)*speed);
	mesh.getPosition()+=front;
}

void Character::update()
{
	if(!active) return;

	for(int i=0; i<MAX_CHARACTER_BOMBS; i++)
	{
		if(bombs[i] && !bombs[i]->isActive())
		{
			delete bombs[i]; bombs[i]=NULL;
			bombsLeft++;
		}
	}

	handleInput();
	rotationAngle=0;

	if(initAction)
	{
		if(blender.done)
		{
			blender.blend(actionA,((a->weight==1)? a:b),&anims[stand],0.2f, true);
			initAction=false;
			actionLasts=true;
			anims[actionType].play();
		}
	}

	if(actionLasts)
	{
		if(blender.done)
		{
			actionLasts=false;
			anims[actionType].stop();

			//if(actionType==putBomb)BombManager::addBomb(bres,mesh.getPosition(),&mesh.getFront());
			if(actionType==putBomb && bombsLeft>0)
			{
				for(int i=0; i<MAX_CHARACTER_BOMBS; i++)
				{
					if(!bombs[i])
					{
						bombs[i] = new BasicBomb(Vec3(mesh.getPosition()[0],0,mesh.getPosition()[2]),5);
						if(!bombs[i]->isActive())
						{
							delete bombs[i]; bombs[i]=NULL;
						}
						else bombsLeft--;
						break;
					}
				}
			}

			if(actionType==die)
			{
				cout<<"aaa";
				alive=false; active=false;
			}

			actionType = none;
		}
	}

	if(!actionLasts && !initAction)
	{
		if(blender.done)
		{
			//PROCEDURA ANIMACJI.
			if(keystate[keys[moveForward]])
			{
				blender.blend(&anims[moveForward],((a->weight==1)? a:b),NULL,0.2f, false);
			}
			else if(keystate[keys[moveBackward]])
			{
				blender.blend(&anims[moveBackward],((a->weight==1)? a:b),NULL,0.2f, false);
			}
			else if(keystate[keys[turnLeft]])
			{
				blender.blend(&anims[turnLeft],((a->weight==1)? a:b),NULL,0.4f, false);
			}
			else if(keystate[keys[turnRight]])
			{
				blender.blend(&anims[turnRight],((a->weight==1)? a:b),NULL,0.4f, false);
			}
			else if(keystate[keys[strafeLeft]])
			{
				blender.blend(&anims[strafeLeft],((a->weight==1)? a:b),NULL,0.4f, false);
			}
			else if(keystate[keys[strafeRight]])
			{
				blender.blend(&anims[strafeRight],((a->weight==1)? a:b),NULL,0.4f, false);
			}
			else
			{
				blender.blend(&anims[stand],((a->weight==1)? a:b),NULL,0.2f, false);
			}
		}

		//Inicjalizacja akcji.
		if(keystate[keys[putBomb]] && !forceAction)
		{
			initAction=true;
			actionA=&anims[putBomb];
			actionType=putBomb;
		}

		if(dying)
		{
			initAction=true;
			actionA=&anims[die];
			actionType=die;
		}
	}

	//Podstawowy ruch jest mozliwy tylko, gdy nie trwa akcja.
	if(!actionLasts)
	{
		if(keystate[keys[moveForward]]) walkForward();
		else if(keystate[keys[moveBackward]])
		{
			Vec3 front=mesh.getFront();
			front*=((*delta)*speed);
			mesh.getPosition()-=front;
		}
		if(keystate[keys[turnLeft]] || mouseLeft) rotateLeft();
		else if(keystate[keys[turnRight]] || mouseRight) rotateRight();

		//Zmienna test sluzy skroceniu zapisu.
		char test=keystate[keys[moveForward]]+keystate[keys[moveBackward]];

		if(keystate[keys[strafeLeft]] && !keystate[keys[strafeRight]] && !test)
		{
			Vec3 up(0,1,0);
			Vec3 left = cross(up, mesh.getFront());
			left*=((*delta)*speed)*0.75f;
			mesh.getPosition()+=left;
		}
		if(!keystate[keys[strafeLeft]] && keystate[keys[strafeRight]] && !test)
		{
			Vec3 up(0,1,0);
			Vec3 left = cross(mesh.getFront(),up);
			left*=((*delta)*speed)*0.75f;
			mesh.getPosition()+=left;
		}
	}

	blender.update((*delta));

	if(!blender.done)
	{
		a=blender.a;
		b=blender.b;
	}

	for(unsigned int i=0; i<11; i++)
	{
		if(&anims[i]==&anims[moveBackward]) anims[i].update((*delta)*0.3f);
		if(&anims[i]==&anims[moveForward]) anims[i].update((*delta)*3);
		if(&anims[i]==&anims[putBomb]) anims[i].update((*delta)*10);
		else anims[i].update((*delta));
	}

	for(unsigned int i=0; i<mesh.skeleton->num; i++)
	{
		if(!a->srt[i] && !b->srt[i]) continue;
		else if(a->srt[i] && !b->srt[i]) a->updateSkeleton();
		else if(!a->srt[i] && b->srt[i]) b->updateSkeleton();
		else
		{
			Quat r;

			r=slerp(a->srt[i]->r,b->srt[i]->r,b->weight);

			Vec3 t=((a->srt[i]->t)*a->weight);
			Vec3 v=(b->srt[i]->t*b->weight);
			t+=v;

			(*mesh.skeleton->wtbuf[i])=r.toMatrix();
			mesh.skeleton->wtbuf[i]->m[3]=t[0];
			mesh.skeleton->wtbuf[i]->m[7]=t[1];
			mesh.skeleton->wtbuf[i]->m[11]=t[2];
		}
	}

	mesh.skeleton->update();
	mesh.skeleton->updateAttributeMatrices();
}

void Character::draw()
{
	if(!active) return;

	mesh.draw();

	if(suit)
	{
		suit->tmat = mesh.tmat;
		suit->draw();
	}
}

/**********************************************************************
 Wczytuje konfiguracje kluczy.
**********************************************************************/
bool Character::readKeyConfig(std::string fileName)
{
	ifstream file(fileName.c_str());

	if(!file.is_open())
	{
		writeErr("Nie mozna otworzyc pliku: "+fileName);
		return false;
	}

	string name, value;

	while(!file.eof())
	{
		file>>name>>value;

		if(name=="MOVE_FORWARD") keys[moveForward]=keyValue(value);
		if(name=="MOVE_BACKWARD") keys[moveBackward]=keyValue(value);
		if(name=="TURN_LEFT") keys[turnLeft]=keyValue(value);
		if(name=="TURN_RIGHT") keys[turnRight]=keyValue(value);
		if(name=="STRAFE_LEFT") keys[strafeLeft]=keyValue(value);
		if(name=="STRAFE_RIGHT") keys[strafeRight]=keyValue(value);
		if(name=="PUT_BOMB") keys[putBomb]=keyValue(value);
	}

	return true;
}

/**********************************************************************
 Odczytuje wartosc klucza na podstawie podanej nazwy. Moze to byc
 klucz typu w,s,a,d lub up,down,left,right. Zwracana jest odpowiednia
 wartosc SDLKey.
**********************************************************************/
SDLKey Character::keyValue(std::string value)
{
	//Zamiana slownej wartosci klucza na male znaki.
	std::transform(value.begin(), value.end(), value.begin(), (int(*)(int))tolower);

	if(value.length() > 1)	//Klucz specjalny, np 'up'.
	{
		if(value=="up") return SDLK_UP;
		if(value=="down") return SDLK_DOWN;
		if(value=="left") return SDLK_LEFT;
		if(value=="right") return SDLK_RIGHT;
		if(value=="space") return SDLK_SPACE;
		if(value=="insert") return SDLK_INSERT;
		if(value=="home") return SDLK_HOME;
		if(value=="end") return SDLK_END;
		if(value=="pageup") return SDLK_PAGEUP;
		if(value=="pagedown") return SDLK_PAGEDOWN;
		if(value=="rshift") return SDLK_RSHIFT;
		if(value=="lshift") return SDLK_LSHIFT;
		if(value=="rctrl") return SDLK_RCTRL;
		if(value=="lctrl") return SDLK_LCTRL;
		if(value=="ralt") return SDLK_RALT;
		if(value=="lalt") return SDLK_LALT;
	}
	return SDLKey((int)value[0]);
}
