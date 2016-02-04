#include "Material.h"

#include <iostream>
//#include <IL/ilut.h>

using namespace std;

Material::Material()
{
	numFaces=0;
	glImage=0;
	memset(data, 0, sizeof(float)*11);
}

Material::~Material()
{
	numFaces=0;
	glImage=0;
	memset(data, 0, sizeof(float)*11);
}

bool Material::load(fstream &file, std::string texDir)
{
	file.read((char*)&numFaces,sizeof(int)); //Dla ilu scian nalozyc material.

	file>>name; file.get();		//Odczytanie nazwy materialu,
	file>>texName; file.get();	//Odczytanie nazwy tekstury.

	file.read((char*)data,sizeof(float)*11); //Odczytanie wlasciwosci materialu.

	if(texName=="x") glImage=0;
	else glImage=TextureManager::get(texName);

	return true;
}

void Material::apply()
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,data);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,data+4);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,data+5);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,data+8);
	glBindTexture(GL_TEXTURE_2D, glImage);
}
