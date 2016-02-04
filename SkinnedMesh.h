#ifndef SKINNEDMESH_H_
#define SKINNEDMESH_H_

#define WRITE_ALL_LOGS

#include <GL/glew.h>
#include <SDL_OpenGL.h>
#include "Mesh.h"
#include "Skeleton.h"
#include "CGShader.h"

class SkinnedMesh : public Mesh
{
	public:

		SkinnedMesh();
		virtual ~SkinnedMesh();
		virtual bool load(std::string fileName, std::string texDir=".");
		virtual void setSkeleton(Skeleton *skel) {skeleton=skel;}
		virtual bool loadSkeleton(std::string fileName);
		virtual void loadSkinningShader(std::string fileName);
		virtual void draw();

	//protected:
		GLuint *avbo;	//Uchwyty do VBO.
		CGparameter bones;
		unsigned int wmax;
		float *abuf;
		CGShader vertex;
		Skeleton *skeleton;

		bool ownSkeleton;

		void loadAttributes(std::fstream &file);
};

#endif /*SKINNEDMESH_H_*/
