#include "CGShader.h"
#include <algorithm>

using namespace std;

CGprofile CGShader::vprofile;
CGprofile CGShader::fprofile;
CGcontext CGShader::context = NULL;
bool CGShader::initialized = false;

CGShader::CGShader()
{
	program=NULL;
	good=false;
}

CGShader::~CGShader()
{
	if(cgIsProgram(program)) cgDestroyProgram(program);
}

/**********************************************************************
 Tworzy obiekt shader'a na podstawie nazwy podanego pliku. Typ jest
 okreslany na podstawie rozszerzenia: .fs (fragment shader) lub .vs
 (vertex shader). Punktem wejsciowym shadera (glowna funkcja) jest
 funkcja o nazwie podanej jako drugi argument. Domyslnie ma on
 wartosc 'main'.
**********************************************************************/
bool CGShader::create(string fileName, string entryPoint)
{
	if(!initialized)
	{
		writeErr("Nie mozna stworzyc obiektu shadera poniewaz klasa "
				 "CGShader nie zostala zainicjalizowana. W celu inicjalizacji "
				 "klasy, nalezy wywolac metode CGShader::init().");

		return false;
	}

	string errorString, ext;

	//Pobranie rozszerzenia pliku i zamiana na male litery.
	ext=fileName.substr(fileName.find_last_of('.'));
	std::transform(ext.begin(),ext.end(),ext.begin(),(int(*)(int))tolower);

	if(ext==".vs") profile=vprofile;
	else if(ext==".fs") profile=fprofile;
	else
	{
		writeErr("Wystapil blad podczas tworzenia obiektu CGShader. Podano nieprawidlowe "
				 "rozszerzenie pliku zrodlowego. Dopuszczalne rozszerzenia to: .vs (VERTEX SHADER) "
				 "lub .fs (FRAGMENT SHADER");
		return false;
	}

	//Proba stworzenia programu.
    program=cgCreateProgramFromFile(context,CG_SOURCE,fileName.c_str(),profile,entryPoint.c_str(),NULL);
	if(checkForCgError(errorString))
	{
		writeErr("Wystapil blad podczas tworzenia obiektu CGShader:<br>"+errorString);
		return false;
	}

	cgGLLoadProgram(program);
	if(checkForCgError(errorString))
	{
		writeErr("Wystapil blad podczas ladowania obiektu CGShader:<br>"+errorString);
		return false;
	}

	writeInf("Pomyslnie stworzono nowy shader: "+fileName);

	good=true;
	return true;
}

/**********************************************************************
 Aktywuje shader, dolaczajac go do potoku graficznego.
**********************************************************************/
void CGShader::begin()
{
	if(!good) return;

	cgGLBindProgram(program);
	cgGLEnableProfile(profile);
}

/**********************************************************************
 Wylacza shader.
**********************************************************************/
void CGShader::end()
{
	if(!good) return;

	cgGLDisableProfile(profile);
}


/**********************************************************************
 METODA STATYCZNA
 Inicjalizuje klase CGShader.
**********************************************************************/
bool CGShader::init()
{
	string errorString;

	//Stworzenie kontekstu CG.
	context=cgCreateContext();

	if(CGShader::checkForCgError(errorString))
	{
		writeErr("Wystapil blad podczas tworzenia kontekstu CG:<br>"+errorString);
		return false;
	}

	cgGLSetDebugMode( CG_FALSE );

	cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);

	//Pobieranie profilu shadera wierzcholkow.
	vprofile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(vprofile);
	if(checkForCgError(errorString))
	{
		writeErr("Wystapil blad podczas pobierania profilu CG_GL_VERTEX:<br>"+errorString);
		return false;
	}

	//Pobieranie profilu shadera wierzcholkow.
	fprofile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(fprofile);
	if(checkForCgError(errorString))
	{
		writeErr("Wystapil blad podczas pobierania profilu CG_GL_FRAGMENT:<br>"+errorString);
		return false;
	}

	initialized=true;
	return true;
}

/**********************************************************************
 METODA STATYCZNA
 Bezpiecznie zwalnie utworzony kontekst CG..
**********************************************************************/
void CGShader::release()
{
	if(cgIsContext(context)) cgDestroyContext(context);
}

/**********************************************************************
 METODA STATYCZNA
 Sprawdza, czy wystapil blad cg. W przypadku bledu zwracane jest true,
 w przeciwnym wypadku false.
**********************************************************************/
bool CGShader::checkForCgError(string &errorString)
{
	CGerror cgError;

	const char *err=cgGetLastErrorString(&cgError);

	if(!err && cgError == CG_NO_ERROR) return false;

	errorString=err;
	errorString.append("<br>");

    if(cgError == CG_COMPILER_ERROR) errorString.append(cgGetLastListing(context));

    return true;
}
