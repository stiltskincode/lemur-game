#include <Particle.h>

#include <sstream>

Particle::Particle()
{

}

Particle::~Particle()
{

}

void Particle::init(float *d, Camera *c)
{
	delta=d;
	camera=c;
	initialized=true;
}

bool Particle::create(bool active, string texture, float life, float width, float height,
		              Vec3 pos, Vec3 dir, Vec3 bdir, float speed, float fadeStart, float fadeDuration)
{
	if(!initialized) return false;

	this->active=active;
	tex=TGAImage::loadGLTexture(texture);
	this->life=life;
	this->width=width;
	this->height=height;
	this->pos=pos;
	this->dir=dir;
	this->bdir=bdir;
	this->speed=speed;
	this->fadeStart=fadeStart;
	this->fadeDuration=fadeDuration;

	return true;
}

bool Particle::update()
{
	// Jezeli czasteczka nie jest zainicializowana lub nie jest aktywna
	// to opuszczamy metode.
	if(!initialized || !active) return false;

	srand(SDL_GetTicks());

	lifeTime+=(*delta);
	movementTime-=(*delta);

	// Jezeli skonczyl sie czas trwania ruchu.
	if(movementTime<=0)
	{
		dir=bdir;
		//dir[1]+=((rand()%1000)-500)/2000.0f;
		dir.normalize();
		movementTime=(rand()%1500)/1000.0f;
	}
	Vec3 v=dir*(*delta);
	v*=speed;
	pos+=v;

	// Zmniejszamy zycie czasteczki.
	life-=(*delta);

	// Ustawianie polprzezroczystowci czasteczki.
	if(lifeTime>=fadeStart)
	{
		fade=1.0f-((lifeTime-fadeStart)/fadeDuration);
		if(fade<0) fade=0;
	}

	// Jezeli zycie czasteczki sie skonczylo to wyzeruj wszystko.
	if(life<=0)
	{
		active=false;
		life=fade=fadeStart=fadeDuration=width=height=speed=0;
		pos.xyz(0,0,0);
		dir.xyz(0,0,0);
	}

	return true;
}

bool Particle::draw()
{
	// Jezeli czasteczka nie jest zainicializowana lub nie jest aktywna
	// to opuszczamy metode.
	if(!initialized || !active) return false;

	glPushMatrix();

		glTranslatef(pos[0],pos[1],pos[2]);
		float angle1, angle2,angle3;
		Vec3 a(1,0,0), b(0,1,0), c(0,0,1), camFront((*camera).getFront());
		camFront.normalize();

		// Liczymy kat obrotu czasteczki w strone kamery.
		angle1=(acos(dot(a,camFront))*180.0f)/3.14159;
		angle2=(acos(dot(b,camFront))*180.0f)/3.14159;

		//camFront[1]=0; camFront.normalize();
		//angle3=(acos(dot(c,camFront))*180.0f)/3.14159;

		// Korygujemy kat obrotu.
		if(camFront[2] < 0) angle1+=90.0f;
		else angle1=90-angle1;

		//if(camFront[2] < 0) angle3-=180.0f;
		//else angle3=180.0f-angle3;

		//stringstream ss;
		//ss<<angle3;
		//SDL_WM_SetCaption(ss.str().c_str(),NULL);
		glRotatef(angle1,0,1,0);
		glRotatef(angle2-90,1,0,0);
		//glRotatef(angle3,0,0,1);



		glEnable( GL_BLEND );
		glDisable(GL_LIGHTING);
		glDisable( GL_LIGHT0 );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glDepthMask(GL_FALSE);
		//glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D,tex);
		glColor4f(1,1,1,0.05f);
		glBegin(GL_QUADS);
			glTexCoord2f(1,0); glVertex3f(-width/2,-height/2,0);
			glTexCoord2f(1,1); glVertex3f(-width/2, height/2,0);
			glTexCoord2f(0,1); glVertex3f( width/2, height/2,0);
			glTexCoord2f(0,0); glVertex3f( width/2,-height/2,0);
		glEnd();
		glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		glDisable( GL_BLEND );
	    //glEnable(GL_LIGHTING);
	    //glEnable( GL_LIGHT0 );

	glPopMatrix();

	return true;
}


