#include "GLShaderProgram.h"

using namespace std;

GLShaderProgram::GLShaderProgram()
{
	numShaders=0;
	shaders = new GLShader*[MAX_SHADERS];
	for(int i=0; i<MAX_SHADERS; i++) shaders[i]=NULL;
	ready=false;
}

GLShaderProgram::~GLShaderProgram()
{
	for(int i=0; i<MAX_SHADERS; i++)
	{
		if(!shaders[i]) continue;

		glDetachObjectARB(program, *shaders[i]);
		delete shaders[i];
		shaders[i]=NULL;
	}

	delete[] shaders; shaders=NULL;
	if(glIsProgram(program)) glDeleteObjectARB(program);
}

bool GLShaderProgram::link()
{
	if(!ready)
	{
		writeErr("Nie mozna zlikowac programu, powiewaz nie "
				 "zostal on poprawnie stworzony!");
		return false;
	}

	glLinkProgramARB(program);

    int length = 0;
    int charsWritten  = 0;
    char *infoLog = NULL;

    //Pobranie dlugosci komunikatu kompilacji shader'a.
    glGetObjectParameterivARB(program,GL_OBJECT_INFO_LOG_LENGTH_ARB,&length);
	if(length>1)	//Dlugosc 0 oznacza brak bledow.
	{
		infoLog=new char[length];
		glGetInfoLogARB(program, length, &charsWritten, infoLog);	//Pobranie log'a.
		string error(infoLog);

		writeErr("Blad w metodzie: link()\n"
			"Nazwa obiektu: "+name+"\n"
			"Wystapil blad przy linkowaniu programu:\n"+error);

		delete[] infoLog; infoLog=NULL;

		return false;
	}

	return true;
}

bool GLShaderProgram::create(string name)
{
	//Proba stworzenia programu shader'a.
	program = glCreateProgramObjectARB();

	if(program==0) //Jezeli nie udalo sie stworzyc programu.
	{
		writeErr("Blad w konstruktorze: GLShaderProgram()\n"
			"Nie mozna stworzyc prorgamu shader'a.");

		ready=false;
		return false;
	}

	this->name=name;

	writeInf("Pomyslnie stworzono obiekt GLShaderProgram o nazwie: "+name);

	ready = true;
	return true;
}

bool GLShaderProgram::addShader(GLShader *shader)
{
	if(ready==false)
	{
		writeErr("BLAD! Nie mozna dodac shadera, poniewaz"
			"program nie zostal poprawnie stworzony!");
		return false;
	}

	for(int i=0; i<MAX_SHADERS; i++)
	{
		if(shaders[i]) continue;

		shaders[i]=shader;
		glAttachObjectARB(program,*shader);	//Dolaczenie shadera do programu.
		return true;
	}

	return false;
}

bool GLShaderProgram::addShader(std::string fileName)
{
	if(ready==false)
	{
		writeErr("BLAD! Nie mozna dodac shadera, poniewaz "
			"program nie zostal poprawnie stworzony!");
		return false;
	}

	GLShader *shader = new GLShader(fileName);
	if(shader->ready)
	{
		addShader(shader);

		writeInf("Pomyslnie dodano now shader do programu");
		return true;
	}
	else
	{
		delete shader; shader=NULL;
		return false;
	}
}

bool GLShaderProgram::removeShader(std::string name)
{
	for(int i=0; i<MAX_SHADERS; i++)
	{
		if(shaders[i] && shaders[i]->name==name)
		{
			glDetachObjectARB(program, *shaders[i]);
			delete shaders[i];
			shaders[i]=NULL;

			return true;
		}
	}
	return false;
}

bool GLShaderProgram::removeShader(int id)
{
	if(id<0 || id>MAX_SHADERS || !shaders[id]) return false;

	glDetachObjectARB(program, *shaders[id]);
	delete shaders[id];
	shaders[id]=NULL;

	return true;
}

/**********************************************************************
 Metoda wlacza lub wylacza program.
**********************************************************************/
void GLShaderProgram::setEnabled(bool value)
{
	if(!ready) return;
	enabled=value;
	if(value)glUseProgramObjectARB(program);
	else glUseProgramObjectARB(0);
}

bool GLShaderProgram::isEnabled()
{
	return enabled;
}

void GLShaderProgram::setUniform(std::string name, GLint v1)
{
	GLint location = glGetUniformLocationARB(program, name.c_str());
	if(location==-1) return;
	glUniform1iARB(location, v1);
}

void GLShaderProgram::setUniform(std::string name, float v1)
{
	GLint location = glGetUniformLocationARB(program, name.c_str());
	if(location==-1) return;
	glUniform1fARB(location, v1);
}

void GLShaderProgram::setUniform(std::string name, float v1, float v2)
{
	GLint location = glGetUniformLocationARB(program, name.c_str());
	if(location==-1) return;
	glUniform2fARB(location, v1, v2);
}

void GLShaderProgram::setUniform(std::string name, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocationARB(program, name.c_str());
	if(location==-1) return;
	glUniform3fARB(location, v1, v2, v3);
}

void GLShaderProgram::setUniform(std::string name, float v1, float v2, float v3, float v4)
{
	GLint location = glGetUniformLocationARB(program, name.c_str());
	if(location==-1) return;
	glUniform4fARB(location, v1, v2, v3, v4);
}
//GLShader* getShader(std::string name);
//GLShader* getShader(int id);
