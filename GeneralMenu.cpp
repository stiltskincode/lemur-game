#include "GeneralMenu.h"

using namespace std;

GeneralMenu::GeneralMenu()
{
	diffX=diffY=1;
	cursor=0;
	active = true;
	elementName = "GeneralMenu";
	nextElement = "Dimental";
}

GeneralMenu::~GeneralMenu()
{

}

void GeneralMenu::update()
{
	if(EventManager::keyDown(SDLK_UP))
	{
		cursor--;
		if(cursor<0) cursor=3;
		SoundManager::play("menu_click");
	}
	if(EventManager::keyDown(SDLK_DOWN))
	{
		cursor++;
		if(cursor>3) cursor=0;
		SoundManager::play("menu_click");
	}
	if(EventManager::keyDown(SDLK_RETURN))
	{
		SoundManager::play("menu_click2");
		switch(cursor)
		{
			case 0:
				nextElement="Scene";
				//active=false;
				ending = true;
				starting = false;
			break;

			case 2:
				nextElement="Authors";
				//active=false;
				ending = true;
				starting = false;
			break;

			case 3:
				nextElement="Exit";
				//active=false;
				ending = true;
				starting = false;
			break;
		}
	}
}

/*void GeneralMenu::setFont(string config, string texture)
{
	font.create(config,texture);
	for(int i=0; i<4; i++)
	{
		text[i].setFont(&font);
	}
    loadMenuResource();
} */

void GeneralMenu::load(std::string location)
{
	active = true;

	this->scrX=GameEngine::scrWidth;
	this->scrY=GameEngine::scrHeight;
	diffX = (((100*float(scrX))/800)/100);
	diffY = (((100*float(scrY))/600)/100);

	mesh.load("lemur/mesh.dim", "lemur/textures");
	mesh.loadSkinningShader("lemur/vertex.vs");
	mesh.loadSkeleton("lemur/skeleton.dim");
	mesh.skeleton->update();
	mesh.skeleton->updateAttributeMatrices();
	anim.load("lemur/animations/stand.dim");
	anim.bind(mesh.skeleton);

	for(int i=0;i<4;i++) strings[i].setResourceFile("POL.dim");
	string keys[4] = { "NEW_GAME", "OPTIONS", "CREDITS", "EXIT_GAME" };
	for(int i=0;i<4;i++)
		strings[i].read(keys[i]);

	tex=TGAImage::loadGLTexture(location+"/menu.tga");
	tex2=TGAImage::loadGLTexture(location+"/shadow.tga");
	tex3=TGAImage::loadGLTexture(location+"/krab.tga");
	tlo = TGAImage::loadGLTexture(location+"/tlo.tga",false);

	SoundManager::addSound(location+"/click.wav","menu_click");
	SoundManager::addSound(location+"/click2.wav","menu_click2");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0);
}

void GeneralMenu::draw()
{
	if(!active) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, scrX, scrY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); gluOrtho2D(0,scrX,0,scrY);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D,tlo);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(0,0,0); 		glTexCoord2f(1,0); glVertex3f(scrX,0,0);
		glTexCoord2f(1,1); glVertex3f(scrX,scrY,0); glTexCoord2f(0,1); glVertex3f(0,scrY,0);
    glEnd();

    // MENU
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPushMatrix();
    glTranslatef(400*diffX,0,0);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(400*diffX,0,0);
		glTexCoord2f(1,1); glVertex3f(400*diffX,600*diffY,0);
		glTexCoord2f(0,1); glVertex3f(0,600*diffY,0);
    glEnd();
    glTranslatef(-400*diffX,0,0);
    glPopMatrix();
    // CIEN POD LEMUREM
    glPushMatrix();
    glTranslatef(60*diffX,90*diffX,0);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
        glTexCoord2f(1,0); glVertex3f(300*diffX,0,0);
        glTexCoord2f(1,1); glVertex3f(300*diffX,100*diffY,0);
        glTexCoord2f(0,1); glVertex3f(0,100*diffY,0);
    glEnd();
    glPopMatrix();
    // KRAB
    glPushMatrix();
    glTranslatef(420*diffX,(215-(cursor*65))*diffY,0);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, tex3);
    glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
        glTexCoord2f(1,0); glVertex3f(100*diffX,0,0);
        glTexCoord2f(1,1); glVertex3f(100*diffX,100*diffY,0);
        glTexCoord2f(0,1); glVertex3f(0,100*diffY,0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
		for(int i=0; i<4; i++)
		{
			glTranslatef(520*diffX,(240-(i*63))*diffY,0);
			//glScalef(((float)scrX/800)/1.5f,((float)scrY/600)/1.5f,1);

			if(cursor!=i)
			{
				glPushMatrix();
				glColor3f(1,1,1);
				glScalef(((float)scrX/800)*0.8f,((float)scrY/600)*0.8f,1);
				//glScalef(((float)scrX/800)/1.7f,((float)scrY/600)/1.7f,1);
				strings[i].draw();
				glPopMatrix();
			}
			else
			{
				glPushMatrix();
				glColor3f(0.15f,0.76f,0.85f);
				glScalef(((float)scrX/800)*0.8f,((float)scrY/600)*0.8f,1);
				//glScalef((((float)scrX/800)/1.7f)*1.2f,((float)scrY/600)/1.7f,1);
				strings[cursor].draw();
				glPopMatrix();
			}

			glTranslatef(-520*diffX,-(240-(i*63))*diffY,0);
		}
		glPopMatrix();
    glDisable( GL_BLEND );
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); gluPerspective(45,float(scrX)/float(scrY),1,10000);

    glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //gluLookAt(100,50,100,0,0,0,0,1,0);

        //glColor3f(1,1,1);

    	//glEnable(GL_DEPTH_TEST);

    glPushMatrix();

    gluLookAt(100*diffX,70*diffY,100*diffX,45*diffX,40*diffY,0,0,1,0);

    anim.update((*delta));
    anim.updateSkeleton();

    mesh.skeleton->update();
    mesh.skeleton->updateAttributeMatrices();

    glScalef(((float)scrX/800)*0.5f,((float)scrY/600)*0.5f,((float)scrX/800)*0.5f);

    glRotatef(-125,0,1,0);
    mesh.draw();

    glPopMatrix();

    //Rysowanie quad'a przyciemniajacego.

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); gluOrtho2D(0,scrX,0,scrY);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glColor4f(0,0,0,start);
    //glColor3f(1,0,0);
    glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(scrX,0);
		glVertex2f(scrX,scrY);
		glVertex2f(0,scrY);
    glEnd();

    if(starting) start-=*delta;
    if(ending) start+=*delta;

    if(start<0) {starting = false; start=0; SoundManager::play("menu"); SoundManager::setLoop(true,"menu"); }
    if(start>1) wait-=*delta;
    if(wait<0) {active = false; SoundManager::stop("menu");}

}
