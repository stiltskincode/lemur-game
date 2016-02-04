#include "GLShader.h"
#include <fstream>

#include <iostream>
#include <algorithm>

using namespace std;

GLShader::GLShader()
{
	ready=enabled=false;
}

GLShader::GLShader(std::string fileName)
{
	ready=enabled=false;
	create(fileName);
}

GLShader::~GLShader()
{
	if(glIsShader(shader)) glDeleteObjectARB(shader);
}

/**********************************************************************
 Jest to glowna metoda klasy GLShader. Sluzy ona do wczytania kodu
 shadera z pliku i skompilowania go. Jezeli wszystko pojdzie pomyslnie,
 stworzony zostanie nowy shader opengl, a klasa GLShader, bedzie
 posiadala do niego uchwyt. Kazdy blad przerywa dzialanie funkcji i
 zapisuje odpowiednie logi.
 Dozwolone typy shader'a to:
 GL_VERTEX_SHADER_ARB lub GL_FRAGMENT_SHADER_ARB
**********************************************************************/
bool GLShader::create(string fileName)
{
	ifstream file(fileName.c_str());
	int size = 0, index=0;
	char *source=NULL, c;

	if(!file.is_open())	//Sprawdzenie czy udalo sie otworzyc plik.
	{
		writeErr("Nie mozna otworzyc pliku "+fileName);
		return false;
	}

	file.seekg(0,ios::end);	//Ustawienie wskaznika odczytu na koniec pliku.
	size=file.tellg();		//Pobranie pozycji wskaznika (rozmiar pliku).
	file.seekg(0,ios::beg);	//Ustawienie wskaznika odczytu na poczatek pliku.

	source = new char[size+1];	//Alokacja pamieci do przechowania zrodla shader'a.

	while(true)
	{
		c=file.get();			//Odczytanie znaku z pliku.
		if(file.eof()) break;	//Koniec petli, jezeli przeczytano caly plik.
		source[index++]=c;		//Zapisanie odczytanego znaku.
	}
	source[index]='\0';	//Wstawienie znaku NULL.

	file.close();

	for(int i=0; i<size; i++) cout<<source[i];

	//Pobranie rozszerzenia pliku i zamiana na male litery.
	string ext=fileName.substr(fileName.find_last_of('.'));
	std::transform(ext.begin(),ext.end(),ext.begin(),(int(*)(int))tolower);

	if(ext==".vs") type=GL_VERTEX_SHADER_ARB;
	else if(ext==".fs") type=GL_FRAGMENT_SHADER_ARB;
	else
	{
		writeErr("Nazwa pliku: "+fileName+"<br>"
				 "Wystapil blad podczas tworzenia obiektu GLShader. Podano nieprawidlowe "
				 "rozszerzenie pliku zrodlowego. Dopuszczalne rozszerzenia to: .vs (VERTEX SHADER) "
				 "lub .fs (FRAGMENT SHADER");
		return false;
	}

	//Proba stworzenia obiektu shader'a.
	shader = glCreateShaderObjectARB(type);
	if(shader==0)
	{
		string error((const char*)gluErrorString(glGetError()));
		writeErr("Nazwa pliku: "+fileName+"<br>"
				 "Wystapil blad przy tworzeniu obiektu shader'a:<br>"+error);
		return false;
	}

	//Proba skopiowania odczytanego z pliku zrodla shader'a.
	glShaderSourceARB(shader,1,(const GLcharARB**)&source,NULL);
	if(glGetError()!=GL_NO_ERROR)
	{
		string error((const char*)gluErrorString(glGetError()));
		writeErr("Nazwa pliku: "+fileName+"<br>"
				 "Wystapil blad przy ustawiania zrodla shader'a: "+error);
		return false;
	}

	delete[] source; source=NULL;	//Zwolnienie pamieci na zrodlo shader'a.

	//Proba skompilowania shader'a.
	glCompileShaderARB(shader);

    int result = 0;
    int charsWritten  = 0;
    char *infoLog = NULL;

    //Pobranie statusu kompilacji shadera.
    glGetShaderiv(shader,GL_COMPILE_STATUS,&result);

    if(result != GL_TRUE)
    {
        //Pobranie dlugosci komunikatu kompilacji shader'a.
        glGetObjectParameterivARB(shader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&result);

        if(result>0)
        {
			infoLog=new char[result];
			glGetInfoLogARB(shader, result, &charsWritten, infoLog);	//Pobranie log'a.
			string error(infoLog);

			writeErr("Nazwa pliku: "+fileName+"<br>"
					 "Wystapil blad kompilacji shader'a:\n"+error);

			delete[] infoLog; infoLog=NULL;
        }
        writeErr("Nazwa pliku: "+fileName+"<br>"
				 "Wystapil blad przy kompilacji shader'a.");

    	return false;
    }

	writeInf("Nazwa pliku: "+fileName+"<br>"
			 "Pomyslnie stworzono nowy obiekt GLShader");

	ready=true;
	return true;
}

void GLShader::setEnabled(bool value)
{
	if(ready)enabled=value;
}
