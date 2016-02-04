/*
Klasa ta stanowi podstawowa reprezentacje obiektu, jakim jets shader w OpenGL.
Posiada ona uchwyt do shadera: GLhandleARB shader, nazwe oraz zmienne informujace
o stanie (ready, enabled). Glowna metoda klasy jest metoda:
bool create(std::string fileName,int type);
*/

#ifndef GLSHADER_H_
#define GLSHADER_H_

#define WRITE_ALL_LOGS

#include <GL/glew.h>
#include <string>
#include "Object.h"
#include "LogWriter.h"

class GLShader : public Object
{
	public:

		friend class GLShaderProgram;

		GLShader();
		GLShader(std::string fileName);
		virtual ~GLShader();

		bool create(std::string fileName);
		void setEnabled(bool value);

		operator GLhandleARB() {return shader;}

	private:
		GLhandleARB shader;
		GLuint type;
		bool ready;
		bool enabled;
};

#endif /*GLSHADER_H_*/
