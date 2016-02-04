#ifndef MATERIAL_H_
#define MATERIAL_H_


#include <GL/glew.h>
#include <fstream>
#include <string>
#include <SDL_opengl.h>
#include "TextureManager.h"
#include "Object.h"

class Material : public Object
{
	friend class Mesh;
	friend class StaticMesh;
	friend class SkinnedMesh;

	public:
		Material();
		~Material();
		bool load(std::fstream &file, std::string texDir=".");
		void apply();
		unsigned int getNumFaces() {return numFaces;}

	private:
		std::string texName;
		unsigned int numFaces;
		float data[11];
		GLuint glImage;
};

#endif /*MATERIAL_H_*/
