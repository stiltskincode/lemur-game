#pragma once

#define WRITE_ALL_LOGS


#include <string>
#include <SDL_OpenGL.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "LogWriter.h"

class CGShader
{
	public:
		CGShader();
		virtual ~CGShader();

		bool create(std::string fileName, std::string entryPoint="main");
		void begin();
		void end();

		CGprogram getProgram() {return program;}
		CGprofile getProfile() {return profile;}

		static bool init();
		static void release();
		static bool checkForCgError(std::string &errorString);
		static CGcontext getContext() {return context; }
		static CGprofile getVertexProfile() {return vprofile; }
		static CGprofile getFragmentProfile() {return fprofile; }

	private:
		CGprogram program;
		CGprofile profile;
		bool good;

		static CGprofile vprofile, fprofile;
		static CGcontext context;
		static bool initialized;
};
