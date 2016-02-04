#include "GameEngine.h"

int main(int argc, char **argv)
{
	LogWriter::begin("log.html", true);
	GameEngine::init();
	GameEngine::run();
	GameEngine::cleanup();
	LogWriter::end();

	return 0;
}
