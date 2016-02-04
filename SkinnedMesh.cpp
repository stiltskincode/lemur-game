#include "SkinnedMesh.h"
#include <sstream>

using namespace std;

SkinnedMesh::SkinnedMesh()
{
	wmax=0;
	abuf=NULL;
	avbo=NULL;
	front.xyz(0,0,-1);
	position.wrap(&tmat[12]);

	skeleton=NULL;
	ownSkeleton = false;
}

SkinnedMesh::~SkinnedMesh()
{
	for(unsigned int i=0; i<wmax; i++)
		if(glIsBufferARB(avbo[i])) glDeleteBuffersARB(1,&avbo[i]);

	if(abuf) { delete[] abuf; abuf=NULL; }
	if(avbo) { delete[] avbo; avbo=NULL; }

	if(ownSkeleton && skeleton) delete skeleton; skeleton=NULL;
}

/**********************************************************************
 Wczytuje mesh z pliku.
**********************************************************************/
bool SkinnedMesh::load(std::string fileName, std::string texDir)
{
	fstream file(fileName.c_str(),ios::binary|ios::in);

	if(!file.is_open()) return false;

	loadBase(file,texDir);
	loadAttributes(file);

	return true;
}

/**********************************************************************
 Wczytuje szkielet (hierarchie kosci) dla danego modelu.
**********************************************************************/
bool SkinnedMesh::loadSkeleton(std::string fileName)
{
	skeleton = new Skeleton;
	skeleton->load(fileName);
	ownSkeleton=true;
	return true;
}

void SkinnedMesh::draw()
{
	glPushMatrix();

		glMultMatrixf(tmat.m);

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

		//skinning.setEnabled(true);
		vertex.begin();

		//glUniformMatrix4fv(bloc,skeleton.num,GL_FALSE,skeleton.abuf);
		cgGLSetMatrixParameterArrayfc(bones,0,skeleton->num,skeleton->abuf);

		for(unsigned int i=0; i<wmax; i++)
		{
		    glActiveTexture (GL_TEXTURE0+i+1);
		    glClientActiveTexture (GL_TEXTURE0+i+1);
		    glEnable(GL_TEXTURE_2D);
		    glBindBufferARB(GL_ARRAY_BUFFER_ARB,avbo[i]);
		    glTexCoordPointer (2, GL_FLOAT, 0, 0);
		    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

		glColor3f(1,1,1);
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,0);

		unsigned int offset=0;
		for(unsigned int i=0; i<mnum; i++)
		{
			mbuf[i].apply();

			glDrawArrays(GL_TRIANGLES,offset,mbuf[i].numFaces*3);
			offset+=(mbuf[i].numFaces*3);
		}

		vertex.end();
		//skinning.setEnabled(false);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

	glPopMatrix();
}

void SkinnedMesh::loadSkinningShader(std::string fileName)
{
	vertex.create(fileName);

	bones=cgGetNamedParameter(vertex.getProgram(),"bones");
	if(!bones) writeErr("Nie mozna pobrac atrybutu: bones");
}

/**********************************************************************
 Wczytuje bufor atrybutow dla mesh'a z pliku. Bufor atrybutow ma
 postac: indeks macierzy, waga, indeks macierzy, waga...
**********************************************************************/
void SkinnedMesh::loadAttributes(fstream &file)
{
	//Maksymalna ilosc kosci, wplywajacych na wierzcholek.
	file.read((char*)&wmax,sizeof(int));

	abuf=new float[vnum*wmax*2];
	file.read((char*)abuf,sizeof(float)*wmax*vnum*2);

	//Przygotowanie VBO dla buforow atrybutow.
	avbo=new GLuint[wmax];

	glGenBuffersARB(wmax, avbo);

	unsigned int offset=0;
	for(unsigned int i=0; i<wmax; i++)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,avbo[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,vnum*2*sizeof(float),abuf+offset,GL_STATIC_DRAW_ARB);
		offset+=vnum*2;
	}

	//Deaktywacja buforow.
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

	if(abuf) { delete[] abuf; abuf=NULL; }
}
