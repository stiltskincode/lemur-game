#include <stdio.h>
#include "TGAImage.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

TGAImage::TGAImage()
{
	data=NULL;
}

TGAImage::~TGAImage()
{
	if(data) {delete[] data; data=NULL;}
}

bool TGAImage::load(const char *fileName)
{
	//Jezeli podano niewlasciwa nazwe pliku (pusty wskaznik)
	if(!fileName)
	{
		writeErr("Nie mozna otworzyc pliku: "+string(fileName));
		return false;
	}

	//Sprawdzenie rozszerzenia (typu pliku).
	string ext(fileName);
	ext=ext.substr(ext.length()-4);
	std::transform(ext.begin(),ext.end(),ext.begin(),::tolower);
	if(ext != ".tga")
	{
		writeErr("Nieprawidlowy format pliku: "+ext);
		return false;
	}

	ifstream file(fileName, ios::in|ios::binary);

	//Jezeli nie udalo sie otworzyc pliku o podanej nazwie
	if(!file.is_open()) return false;

	//Odczytanie naglowka pliku (18 bajtow)
	TGAImageHeader header;
	file.read((char*)&header.idLength, 1);
	file.read((char*)&header.colorMapType, 1);
	file.read((char*)&header.type, 1);
	file.read((char*)&header.firstEntryIndex, 2);
	file.read((char*)&header.colorMapLength, 2);
	file.read((char*)&header.colorMapEntrySize, 1);
	file.read((char*)&header.xOrigin, 2);
	file.read((char*)&header.yOrigin, 2);
	file.read((char*)&header.width, 2);
	file.read((char*)&header.height, 2);
	file.read((char*)&header.pixelDepth, 1);
	file.read((char*)&header.imageDescriptor, 1);

	//Pominiecie ID obrazu i mapy kolorow.
	if (header.idLength > 0) file.seekg(header.idLength,ios::cur);
	if (header.colorMapLength > 0) file.seekg(header.colorMapLength,ios::cur);

	//Odczytanie danych obrazu
	bpp = header.pixelDepth/8;
	width = header.width;
	height = header.height;
	data = new unsigned char [width*height*bpp];

	//Okreslenie polozenia obrazu
	switch(header.imageDescriptor>>4)
	{
		case 0x0: origin=BOTTOM_LEFT; break;
		case 0x1: origin=BOTTOM_RIGHT; break;
		case 0x2: origin=TOP_LEFT; break;
		case 0x3: origin=TOP_RIGHT; break;
	}

	switch(header.type)
	{
		//Brak danych o obrazie
		case 0:
		break;

		//Nieskompresowany obraz True-Color
		case 2:
			file.read((char*)data,width*height*bpp);
		break;

		//Skompresowany obraz True-Color
		case 10:
			unsigned char repetitionCount=0, length=0;
			unsigned int pixelCount=0;
			unsigned char *packet=new unsigned char[bpp];

			while(pixelCount<width*height)
			{
				file.read((char*)&repetitionCount,1);

				//To jest Run-length Packet
				if(repetitionCount>=128)
				{
					length=repetitionCount-127;
					file.read((char*)packet,bpp);

					for(unsigned char pixel=0; pixel<length; pixel++)
					{
						for(unsigned char byte=0; byte<bpp; byte++)
						{
							data[pixelCount+byte] = packet[byte];
						}
						pixelCount++;
					}
				}

				//To jest Raw Packet
				else
				{
					length=repetitionCount+1;
					file.read((char*)&data[pixelCount*bpp],bpp*length);
					pixelCount+=length;
				}
			}
			delete[] packet; packet=NULL;
		break;
	}
	//SwapRedAndBlue();
	return true;
}

void TGAImage::swapRedAndBlue()
{
	unsigned char temp=0;

	for(unsigned int i=0; i<width*height*bpp; i+=(int)bpp)
	{
		temp=data[i];
		data[i]=data[i+2];
		data[i+2]=temp;
	}
}

GLuint TGAImage::loadGLTexture(std::string fileName, bool buildMipmaps)
{
	return TGAImage::loadGLTexture(fileName.c_str(), buildMipmaps);
}

GLuint TGAImage::loadGLTexture(const char* fileName, bool buildMipmaps)
{
	TGAImage image;
	GLuint res;
	if(!image.load(fileName)) return 0;

	unsigned int width=image.getWidth();
	unsigned int height=image.getHeight();

	GLint intFormat = GL_RGB;
	GLenum format = GL_BGR;

	if(image.hasAlpha()) {intFormat=GL_RGBA; format=GL_BGRA;}

	glGenTextures(1,&res);				//Wygenerowanie nazwy.
	glBindTexture(GL_TEXTURE_2D,res);	//Aktywowanie nowej tekstury.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(buildMipmaps)
		gluBuild2DMipmaps(GL_TEXTURE_2D,intFormat,width,height,format,GL_UNSIGNED_BYTE,image.getData());
	else
		glTexImage2D(GL_TEXTURE_2D,0,intFormat,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,image.getData());

	return res;
}
