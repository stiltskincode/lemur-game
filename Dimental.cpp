#include "Dimental.h"

Dimental::Dimental()
{
	reset();
	elementName = "Dimental";
}

Dimental::~Dimental()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if(glIsTexture(tex)) glDeleteTextures(1,&tex);
}

void Dimental::reset()
{
	start = stop = 1;
	elapsedTime = 0;
	duration = 5;
	active = true;

	int w = GameEngine::scrWidth;
	int h = GameEngine::scrHeight;
	float aspect = float(w)/float(h);

	SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,aspect,1,1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0,0,0,1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Dimental::load(std::string location)
{
	tex = TGAImage::loadGLTexture(location+"/dimental.tga");
}

void Dimental::draw()
{
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float alpha = sin((elapsedTime/duration)*3.1415963f);

	start -= *delta;
	if(start>0) alpha=0;
	else elapsedTime += *delta;

	if(elapsedTime >= duration ) stop -= *delta;

	if(stop<=0) active = false;

	gluLookAt(0,0,800, 0,0,0, 0,1,0);

	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4f(1,1,1,alpha);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(-180,-93,0);
		glTexCoord2f(1,0); glVertex3f( 180,-93,0);
		glTexCoord2f(1,1); glVertex3f( 180, 93,0);
		glTexCoord2f(0,1); glVertex3f(-180, 93,0);
	glEnd();
}
