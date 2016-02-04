#ifndef BITMAPFONT_H_
#define BITMAPFONT_H_

#define WRITE_ALL_LOGS

#include <GL/glew.h>
#include <SDL_OpenGL.h>
#include <fstream>
#include <string>
#include "Object.h"
#include "TGAImage.h"
#include "LogWriter.h"

class BitmapFont : public Object
{
	public:
		struct BitmapCharacter
		{
			GLuint bitmap;
			unsigned int code, width, height;
			float coord[8];
		};

		BitmapFont();
		virtual ~BitmapFont();
		bool create(std::string configFile, std::string textureFile);
		BitmapCharacter* getCharacter(unsigned int code);
		BitmapCharacter* getEmptyCharacter();
		
	private:
		GLuint bitmap;
		BitmapCharacter *chars, empty;
		unsigned int num;
		bool ready;
};

#endif /*BITMAPFONT_H_*/
