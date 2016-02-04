#include "BitmapString.h"
#include <iostream>

BitmapString::BitmapString()
{
	width=height=num=0;
	font=NULL;
	centered=false;
	for(unsigned int i=0; i<MAX_CHARACTERS; i++) chars[i]=NULL;
}

BitmapString::~BitmapString()
{
	font=NULL;
	for(unsigned int i=0; i<MAX_CHARACTERS; i++) chars[i]=NULL;
}

/**********************************************************************
 Odczytuje napis z pliku. Podczas odczytywania pomijane sa wiodace
 biale znaki, a samo odczytywanie trwa do napotkania bialego znaku.
**********************************************************************/
bool BitmapString::read(std::fstream &file)
{
	if(!font)
	{
		writeErr("BitmapString::read - Czcionka nie zostala ustawiona!");
		return false;
	}

	if(!file.is_open())
	{
		writeErr("BitmapString::read - Plik zrodlowy nie zostal"
				 "otwarty!");
		return false;
	}

	int a=0,b=0;

	//Petla inicjalizujaca - pomija wiadace biale znaki.
	while(!file.eof())
	{
		//Pobranie dwoch bajtow z pliku.
		a=file.get(); b=file.get();

		if(file.eof()) return false;

		//Na poczatku, pomijamy biale znaki.
		if(a==255 && b==254) continue;	//Poczatek pliku.
		if(a==32 && b==0) continue;		//Spacja.
		if(a==13 && b==0) continue;		//Koniec linii 1.
		if(a==10 && b==0) continue;		//Koniec linii 2.

		//Petla czytajaca - odczytuje znaki.
		while(!file.eof())
		{
			//Konczymy, po napotkaniu bialego znaku.
			if(a==32 && b==0) break;	//Spacja.
			if(a==13 && b==0) break;	//Koniec linii 1.
			if(a==10 && b==0) break;	//Koniec linii 2.

			//Sprawdzenie, czy nie przekroczono limitu znakow.
			if(num>=MAX_CHARACTERS)
			{
				writeWar("BitmapString::read - Osiagnieto limit znakow.");
				break;
			}
			chars[num]=font->getCharacter(a+(256*b));
			if(!chars[num])
			{
				writeWar("BitmapString::read - Czcionka nie posiada "
						 "szukanego znaku.");
				chars[num]=font->getEmptyCharacter();
			}
			width+=chars[num]->width;
			height=(height<(int)chars[num]->height)? chars[num]->height:height;
			num++;
			//Pobranie dwoch bajtow z pliku.
			a=file.get(); b=file.get();
		}
		break;
	}

	return true;
}

/**********************************************************************
 Odczytuje linie z pliku. W wypadku tej metody wiodace i konczace
 biale znaki nie sa pomijane. Ogranicznikiem jest znak nowej linii,
 ktory w przypadku unicode to (13 0).
**********************************************************************/
bool BitmapString::readLine(std::fstream &file)
{
	if(!font)
	{
		writeErr("BitmapString::readLine - Czcionka nie zostala ustawiona!");
		return false;
	}

	if(!file.is_open())
	{
		writeErr("BitmapString::readLine - Plik zrodlowy nie zostal"
				 "otwarty!");
		return false;
	}

	int a=0,b=0;

	//Petla inicjalizujaca - pomija wiadace biale znaki.
	while(!file.eof())
	{
		//Pobranie dwoch bajtow z pliku.
		a=file.get(); b=file.get();

		if(file.eof()) return false;

		//Na poczatku, pomijamy biale znaki.
		if(a==255 && b==254) continue;	//Poczatek pliku.
		//if(a==32 && b==0) continue;		//Spacja.
		if(a==13 && b==0) continue;		//Koniec linii 1.
		if(a==10 && b==0) continue;		//Koniec linii 2.

		//Petla czytajaca - odczytuje znaki.
		while(!file.eof())
		{
			//Konczymy, po napotkaniu bialego znaku.
			//if(a==32 && b==0) break;	//Spacja.
			if(a==13 && b==0) break;	//Koniec linii 1.
			if(a==10 && b==0) break;	//Koniec linii 2.
			if(num>=MAX_CHARACTERS) break;

			chars[num]=font->getCharacter(a+(256*b));
			if(!chars[num])
			{
				writeWar("BitmapString::readLine - Czcionka nie posiada "
						 "szukanego znaku: "+(a+(256*b)));
				chars[num]=font->getEmptyCharacter();
			}
			width+=chars[num]->width;
			num++;

			//Pobranie dwoch bajtow z pliku.
			a=file.get(); b=file.get();
		}
		break;
	}

	return true;
}

void BitmapString::setFont(BitmapFont *font)
{
	this->font=font;
}

/**********************************************************************
 Ustawia lancuch znakow na ten, dany jako argument.
**********************************************************************/
void BitmapString::set(std::string text)
{
	this->width = this->height = 0;
	this->centered = false;
	num=0;
	for(unsigned int i=0; i<MAX_CHARACTERS; i++) chars[i]=NULL;
	for(unsigned int i=0; i<text.length(); i++)
	{
		chars[i] = font->getCharacter((int)text[i]);
		width+=chars[i]->width;
		height=(height<(int)chars[i]->height)? chars[i]->height:height;
		num++;
	}
}

void BitmapString::draw(int offset)
{
	glPushMatrix();
	if(centered) glTranslatef(-(width/2),-(height/2),0);
	for(unsigned int i=0; i<num; i++)
	{

		drawCharacter(chars[i]);
		glTranslatef(chars[i]->width+offset,0,0);
	}
	glPopMatrix();
}

void BitmapString::drawCharacter(BitmapFont::BitmapCharacter *c)
{
	glBindTexture(GL_TEXTURE_2D,c->bitmap);
	//glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2f(c->coord[0],c->coord[1]); glVertex3f(0,0,0);
		glTexCoord2f(c->coord[2],c->coord[3]); glVertex3f(c->width,0,0);
		glTexCoord2f(c->coord[4],c->coord[5]); glVertex3f(c->width,c->height,0);
		glTexCoord2f(c->coord[6],c->coord[7]); glVertex3f(0,c->height,0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
}
