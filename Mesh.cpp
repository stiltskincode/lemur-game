#include "Mesh.h"

using namespace std;

Mesh::Mesh()
{
	mbuf=NULL;
	vnum=mnum=0;
	position.wrap(&tmat[12]);
}

Mesh::~Mesh()
{
	if(glIsBufferARB(vvbo)) glDeleteBuffersARB(1,&vvbo);
	if(glIsBufferARB(nvbo)) glDeleteBuffersARB(1,&nvbo);
	if(glIsBufferARB(tvbo)) glDeleteBuffersARB(1,&tvbo);

	SAFE_DELETE_ARRAY(mbuf);
}

/**********************************************************************
 Wczytuje mesh z pliku.
 Ta metoda wywoluje inne metody (zazwyczaj prywatne), ktore w
 zaleznosci od rodzaju mesh'a, moga wczytywac go w inny sposob.
**********************************************************************/
bool Mesh::load(std::string fileName, std::string texDir)
{
	fstream file(fileName.c_str(),ios::binary|ios::in);

	if(!file.is_open()) return false;

	loadBase(file, texDir);

	return true;
}

/**********************************************************************
 Rysuje mesh.
 W przypadku klasy Mesh stosowane sa VertexArrays.
**********************************************************************/
void Mesh::draw()
{
	glPushMatrix();

		glEnableClientState(GL_VERTEX_ARRAY );
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vvbo);
		glVertexPointer(3,GL_FLOAT,0,0);

		glEnableClientState(GL_NORMAL_ARRAY );
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,nvbo);
		glNormalPointer(GL_FLOAT,0,0);

		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,tvbo);
		glTexCoordPointer(2,GL_FLOAT,0,0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

		glMultMatrixf(tmat.m);

		unsigned int offset=0;
		for(unsigned int i=0; i<mnum; i++)
		{
			mbuf[i].apply();

			glDrawArrays(GL_TRIANGLES,offset,mbuf[i].numFaces*3);
			offset+=(mbuf[i].numFaces*3);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

	glPopMatrix();
}

/**********************************************************************
 Wczytuje podstawowa czesc kazdego mesh'a z pliku, czyli bufor
 wierzcholkow razem z atrybutami (normalne i koordynaty) oraz bufor
 materialow.
**********************************************************************/
void Mesh::loadBase(fstream &file, std::string texDir)
{
	float *data, *vbuf, *nbuf, *tbuf;
	data=vbuf=nbuf=tbuf=NULL;

	file>>name; file.get();			//Odczytanie nazwy mesh'a.

	//BUFOR PODSTAWOWY - wierzcholki, normalne, koordynaty.
	file.read((char*)&vnum,sizeof(int));	//Odczytanie ilosci punktow.
	data = new float[vnum*8];
	vbuf=&data[0];
	nbuf=&data[vnum*3];
	tbuf=&data[vnum*6];
	file.read((char*)data,vnum*8*sizeof(float));	//Dane o wierzcholkach.

	//Przygotowanie VBO dla bufora wierzcholkow.
	glGenBuffersARB(1, &vvbo);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vvbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,vnum*sizeof(float)*3,vbuf,GL_STATIC_DRAW_ARB);

	//Przygotowanie VBO dla bufora normalnych.
	glGenBuffersARB(1, &nvbo);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,nvbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,vnum*sizeof(float)*3,nbuf,GL_STATIC_DRAW_ARB);

	//Przygotowanie VBO dla bufora koordynat.
	glGenBuffersARB(1, &tvbo);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,tvbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,vnum*sizeof(float)*2,tbuf,GL_STATIC_DRAW_ARB);

	delete[] data;
	data=vbuf=nbuf=tbuf=NULL;

	//BUFOR MATERIALOW.
	file.read((char*)&mnum,sizeof(int));	//Odczytanie ilosci materialow.
	mbuf = new Material[mnum];
	for(unsigned int i=0; i<mnum; i++) mbuf[i].load(file, texDir);	//Materialy.
}
