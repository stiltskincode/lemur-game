#include "BitmapText.h"

#include <iostream>

using namespace std;

BitmapText::BitmapText()
{
	height=0;
	num=0;
	font=NULL;
	for(unsigned int i=0; i<MAX_LINES; i++) lines[i]=NULL;
}

BitmapText::~BitmapText()
{
	num=0;
	font=NULL;
	for(unsigned int i=0; i<MAX_LINES; i++)
		if(lines[i]) delete lines[i];
}

void BitmapText::setFont(BitmapFont *font)
{
	this->font=font;
}

bool BitmapText::read(std::fstream &file)
{
	if(!font)
	{
		return false;
	}

	if(!file.is_open())
	{
		return false;
	}

	while(!file.eof())
	{
		lines[num] = new BitmapString;
		lines[num]->setFont(font);

		if(!lines[num]->readLine(file))
		{
			delete lines[num];
			lines[num]=NULL;
			break;
		}
		else num++;
	}

	if(height==0) height=lines[0]->chars[0]->width;

	return true;
}

void BitmapText::draw(int offset)
{
	//unsigned int offset=0;
	for(unsigned int i=0; i<num; i++)
	{
		glPushMatrix();
		lines[i]->draw(offset);
		glPopMatrix();
		//offset+=chars[i]->width;
		glTranslatef(0,-40,0);
	}
}
