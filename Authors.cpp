#include "Authors.h"

Authors::Authors()
{
	elementName = "Authors";
	reset();
}

Authors::~Authors()
{

}

void Authors::load(std::string location)
{
	//bf.create(location+"/config.ini",location+"/font.tga");

	//fstream file((location+"/text.txt").c_str(), ios_base::in);
	//bt.setFont(&bf);
	//bt.read(file);

	credits.setResourceFile("POL.dim");
	credits.read("AUTHORS");

	tex = TGAImage::loadGLTexture(location+"/tlo.tga");
}

void Authors::reset()
{
	active = true;
	height = -10;
	alpha=1;
	starting=true;
	ending=false;

	int w = GameEngine::scrWidth;
	int h = GameEngine::scrHeight;

	SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);
	//gluPerspective(45,aspect,1,1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0,0,0,1);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Authors::draw()
{
	if(EventManager::keyDown(SDLK_ESCAPE)) {starting=false; ending=true;}

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	//Tlo
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,tex);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(0,0);
		glTexCoord2f(1,0); glVertex2f(GameEngine::scrWidth,0);
		glTexCoord2f(1,1); glVertex2f(GameEngine::scrWidth,GameEngine::scrHeight);
		glTexCoord2f(0,1); glVertex2f(0,GameEngine::scrHeight);
	glEnd();

	//Napis
	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(1,1,1);
	glTranslatef(100,height,0);
	//bt.draw();

	credits.draw();

	//Quad przyciemniajacy
	glLoadIdentity();
	if(starting)
	{
		alpha-=*delta;
		if(alpha<0) {starting=false; alpha=0;}
	}
	if(ending)
	{
		alpha+=*delta;
		if(alpha>1) {active=false; alpha=1;}
	}
	glColor4f(0,0,0,alpha);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(GameEngine::scrWidth,0);
		glVertex2f(GameEngine::scrWidth,GameEngine::scrHeight);
		glVertex2f(0,GameEngine::scrHeight);
	glEnd();

	height+=(*delta)*30;
}

void Authors::update()
{

}
