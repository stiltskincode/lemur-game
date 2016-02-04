#ifndef STRINGRESOURCES_H_
#define STRINGRESOURCES_H_

#include <string>

#include "BitmapText.h"
#include "Tfp.h"

class StringResources
{
	public:
		StringResources();
		virtual ~StringResources();

		static void loadFonts(std::string location);
		void setResourceFile(std::string fileName);
		void read(std::string key);
		void draw();

	private:
		std::fstream file;
		static BitmapFont font;
		BitmapText text;
		Tfp tfp;
		std::string resource;
};

#endif
