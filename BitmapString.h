#ifndef BITMAPSTRING_H_
#define BITMAPSTRING_H_

#define WRITE_ALL_LOGS
#define MAX_CHARACTERS 256

#include <fstream>
#include "BitmapFont.h"
#include "Object.h"
#include "LogWriter.h"

class BitmapString : public Object
{
	friend class BitmapText;

	public:
		BitmapString();
		virtual ~BitmapString();
		//bool addCharacter(unsigned int code);
		bool read(std::fstream &file);
		bool readLine(std::fstream &file);
		void setFont(BitmapFont *font);
		void set(std::string text);
		void draw(int offset);
		void setCentered(float value) {centered=value;}
		int width, height;


	//private:
		bool centered;
		unsigned int num;

		BitmapFont *font;
		BitmapFont::BitmapCharacter *chars[MAX_CHARACTERS];

		void drawCharacter(BitmapFont::BitmapCharacter *c);
};

#endif /*BITMAPSTRING_H_*/
