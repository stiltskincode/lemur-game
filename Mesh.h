/**********************************************************************
 Klasa Mesh reprezentuje obiekt, zbior punktow w przestrzeni, razem z
 atrybutami, ktory mozna narysowac. Kazdy mesh jest instancja klasy
 Object3D, po ktorej dziedziczy.
**********************************************************************/

#ifndef MESH_H_
#define MESH_H_

#define WRITE_ALL_LOGS

#include <GL/glew.h>

#define SAFE_DELETE(p) {if(p) delete (p); (p)=NULL;}
#define SAFE_DELETE_ARRAY(p) {if(p) delete[] (p); (p)=NULL;}

typedef unsigned int UINT;

#include <fstream>
#include <SDL_OpenGL.h>
#include "Object3D.h"
#include "Mathematics.h"
#include "Material.h"

class Mesh : public Object3D
{
	public:
		Mesh();
		virtual ~Mesh();
		virtual bool load(std::string fileName, std::string texDir=".");
		virtual void draw();

		virtual void rotateY(float angle)
		{
			front.rotate(0,angle,0);
			right=cross(front,up);
			Mat4 r; r.rotationY(angle);
			tmat*=r;
		}
		virtual void setPosition(float x, float y, float z)
		{
			position.xyz(x,y,z);
			tmat[12]=x;
			tmat[13]=y;
			tmat[14]=z;
		}

	//protected:
		GLuint vvbo, nvbo, tvbo;
		unsigned int vnum, mnum;
		Material *mbuf;
		Mat4 tmat;

		void loadBase(std::fstream &file, std::string texDir);
};

#endif /*MESH_H_*/
