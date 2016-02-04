#ifndef GLSHADERPROGRAM_H_
#define GLSHADERPROGRAM_H_

#define WRITE_ALL_LOGS
#define MAX_SHADERS 100

#include <GL/glew.h>
#include <string>
#include "LogWriter.h"
#include "GLShader.h"
#include "Object.h"

class GLShaderProgram : public Object
{
	public:
		GLShaderProgram();
		virtual ~GLShaderProgram();

		bool link();
		bool create(std::string name);
		bool addShader(GLShader *shader);
		bool addShader(std::string fileName);
		bool removeShader(std::string name);
		bool removeShader(int id);
		void removeAll();
		void setEnabled(bool value);
		void setUniform(std::string name, GLint v1);
		void setUniform(std::string name, float v1);
		void setUniform(std::string name, float v1, float v2);
		void setUniform(std::string name, float v1, float v2, float v3);
		void setUniform(std::string name, float v1, float v2, float v3, float v4);
		bool isEnabled();
		GLShader* getShader(std::string name);
		GLShader* getShader(int id);

	//private:
		GLhandleARB program;
		GLShader **shaders;
		int numShaders;
		bool ready;
		bool enabled;
};

#endif /*GLSHADERPROGRAM_H_*/
