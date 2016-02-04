#include "BitmapFont.h"
#include <iostream>

using namespace std;

BitmapFont::BitmapFont()
{
	empty.bitmap=0;
	empty.width=20;

	num=0;
	bitmap=0;
	chars=NULL;
	ready=false;
}

BitmapFont::~BitmapFont()
{
	if(glIsTexture(bitmap)) glDeleteTextures(1,&bitmap);
	if(chars) delete[] chars;
}

/**********************************************************************
 Tworzy obiekt czcionki bitmapowej na podstawie pliku konfiguracyjnego
 .ini oraz pliku z bitmapa. Plik .ini powinien pochodzic z programu
 FontStudio. Przed wykorzystaniem nalezy wstepnie przetworzyc plik
 (edycja/zamien) by zamienic kazdy znak '=' na 3 znaki ' = ' oraz
 kazdy przecinek ',' na kropke '.' 
**********************************************************************/
bool BitmapFont::create(std::string configFile, std::string bitmapFile)
{
	string buffer;
	unsigned int c=0;
	
	fstream file(configFile.c_str(),ios::in);
	
	//Sprawdzenie, czy udalo sie otworzyc plik konfiguracyjny.
	if(!file.is_open())
	{
		writeErr("BitmapFont::create - plik konfiguracyjny: "+configFile+" "
				 "nie zostal znaleziony!");
		return false;
	}
	
	while(!file.eof())
	{
		file>>buffer;
		
		//Odczytanie ilosci znakow.
		if(buffer=="NumFont")
		{
			file>>buffer>>num;
			chars=new BitmapCharacter[num];
			for(unsigned int i=0; i<num; i++)
			{
				chars[i].code=chars[i].width=chars[i].height=0;
				for(unsigned int j=0; j<8; j++) chars[i].coord[j]=0.0f;
			}
		}
		//Odczytanie znaku.
		if(buffer=="Char")
		{
			file>>buffer>>chars[c].code;
			file>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer;
			file>>buffer>>buffer>>chars[c].width;
			file>>buffer>>buffer>>chars[c].height;
			file>>buffer>>buffer>>chars[c].coord[6];	//0 0 x
			file>>buffer>>buffer>>chars[c].coord[7];	//0 0 y
			file>>buffer>>buffer>>chars[c].coord[2];	//1 1 x
			file>>buffer>>buffer>>chars[c].coord[3];	//1 1 y
			
			chars[c].coord[7]=1-chars[c].coord[7];
			chars[c].coord[3]=1-chars[c].coord[3];
			
			chars[c].coord[0]=chars[c].coord[6];		//1 0 x
			chars[c].coord[1]=chars[c].coord[3];		//1 0 y
			chars[c].coord[4]=chars[c].coord[2];		//0 1 x
			chars[c].coord[5]=chars[c].coord[7];		//0 1 y			
			c++;
		}
	}
	
	TGAImage img;
	
	//Sprawdzenie, czy udalo sie wczytac plik z bitmapa.
	if(!img.load(bitmapFile.c_str()))
	{
		writeErr("BitmapFont::create - plik z bitmapa: "+bitmapFile+" "
				 "nie zostal zaladowany!");
		
		if(chars) delete[] chars;
		num=0;
		
		return false;
	}

	unsigned int width=img.getWidth();
	unsigned int height=img.getHeight();
	
	GLint intFormat = GL_RGB;
	GLenum format = GL_BGR;
	
	if(img.hasAlpha()) {intFormat=GL_RGBA; format=GL_BGRA;}

	glGenTextures(1,&bitmap);				//Wygenerowanie nazwy.
	glBindTexture(GL_TEXTURE_2D,bitmap);	//Aktywowanie nowej tekstury.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gluBuild2DMipmaps(GL_TEXTURE_2D,intFormat,width,height,format,GL_UNSIGNED_BYTE,img.getData());
	
	ready=true;
	
	return true;
}

/**********************************************************************
 Pobiera znak o danym kodzie. Jezeli szukany znak nie istnieje,
 zwracany jest NULL.
**********************************************************************/
BitmapFont::BitmapCharacter* BitmapFont::getCharacter(unsigned int code)
{
	for(unsigned int i=0; i<num; i++)	//Dla kazdego znaku.
	{
		if(chars[i].code==code)	//Jezeli znak zostal znaleziony.
		{
			chars[i].bitmap=bitmap;
			return &chars[i];
		}
	}
	
	return NULL;
}

/**********************************************************************
 Zwraca pusty znak.
**********************************************************************/
BitmapFont::BitmapCharacter* BitmapFont::getEmptyCharacter()
{
	return &empty;
}
