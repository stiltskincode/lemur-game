#ifndef TGAIMAGE_H_
#define TGAIMAGE_H_

#define WRITE_ALL_LOGS

#include <GL/glew.h>
#include <SDL_OpenGL.h>
#include <string>
#include "LogWriter.h"

class TGAImage
{
	enum TGAImageOrigin { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

	struct TGAImageHeader
	{
		//Dlugosc ID
		unsigned char idLength;			//1 bajt

		//Typ mapy kolorow
		unsigned char colorMapType;		//1 bajt

		//Typ obrazu (1 bajt)
		unsigned char type;				//1 bajt

		//Specyfikacja mapy kolorow (5 bajtow)
		unsigned short firstEntryIndex;		//2 bajty
		unsigned short colorMapLength;		//2 bajty
		unsigned char colorMapEntrySize;	//1 bajt

		//Specifikacja obrazu
		unsigned short xOrigin;			//2 bajty
		unsigned short yOrigin;			//2 bajty
		unsigned short width;			//2 bajty
		unsigned short height;			//2 bajty
		unsigned char pixelDepth;		//1 bajt
		unsigned char imageDescriptor;	//1 bajt
	};

	public:
		TGAImage();
		~TGAImage();

		bool load(const char*);
		static GLuint loadGLTexture(const char*, bool buildMipmaps = true);
		static GLuint loadGLTexture(std::string, bool buildMipmaps = true);
		void* getData() {return (void*)data;};
		unsigned int getWidth() {return width;};
		unsigned int getHeight() {return height;};
		bool hasAlpha() { return (bpp>3)? true:false;}
		void swapRedAndBlue();

	private:
		unsigned char bpp;
		unsigned char *data;
		unsigned int width, height;
		unsigned short xOrigin, yOrigin;
		TGAImageOrigin origin;
};

#endif
