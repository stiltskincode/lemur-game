#include "StringResources.h"

using namespace std;

BitmapFont StringResources::font;

StringResources::StringResources()
{

}

StringResources::~StringResources()
{

}

void StringResources::loadFonts(std::string location)
{
	font.create("komix.ini","komix.tga");
}

void StringResources::setResourceFile(string fileName)
{
	resource=fileName;
}

void StringResources::read(string key)
{
	string name;
	text.setFont(&font);
	tfp.open(resource);
	tfp.setExtractPath("Temp");

	tfp.decompress(key);
	name="Temp/"+key;
	file.open(name.c_str(),ios::in);
	text.read(file);
	file.clear();
	file.close();
	tfp.deteleDecompressedFiles();
	tfp.close();
}

void StringResources::draw()
{
	text.draw(-3);
}
