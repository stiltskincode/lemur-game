#ifndef BITMAPTEXT_H_
#define BITMAPTEXT_H_

#define MAX_LINES 256

#include "BitmapString.h"

class BitmapText
{
	public:
		BitmapText();
		virtual ~BitmapText();
		void setFont(BitmapFont *font);
		bool read(std::fstream &file);
		void draw(int offset=0);

	private:
		unsigned int num, height;
		BitmapString *lines[MAX_LINES];
		BitmapFont *font;
};

#endif /*BITMAPTEXT_H_*/
