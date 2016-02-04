#include <GameEngine.h>

using namespace std;

bool GameEngine::initialized = true;
float GameEngine::start=0, GameEngine::end=0, GameEngine::delta=0;

Object3D *GameEngine::current  = NULL;
deque<Object3D*> GameEngine::shared;
vector<GameEngine::Desc> GameEngine::desc;

Character GameEngine::lemur;
Layout GameEngine::layout;

int GameEngine::scrWidth=1024, GameEngine::scrHeight=768;

/**********************************************************************
 METODA: PRYWATNA, STATYCZNA
 Zarzadza przechodzeniem pomiedzy elementami.
**********************************************************************/
bool GameEngine::getElement(std::string name)
{
	int id = -1;

	//Jezeli stary obiekt nie jest wspoldzielony, usuwamy go.
	if(current && !current->sharedElement) {delete current; current=NULL;}

	//Odnalezienie opisu dla danego elementu.
	for(unsigned int i=0; i<desc.size(); i++)
	{
		if(desc[i].name != name) continue;
		id=i; break;
	}

	//Jezeli nie znaleziono opisu dla elmentu, zwracane jest NULL.
	if(id==-1) return false;

	//Sprawdzenie, czy szukany element znajduje sie w tablicy elementow
	//wspoldzielonych.
	for(unsigned int i=0; i<shared.size(); i++)
	{
		if(shared[i]->getElementName() == name)
		{
			shared[i]->reset();
			current=shared[i];
			return true;
		}
	}

	//cout<<"Szukany element: "<<name<<"  lokalizacja: "<<desc[id].location<<endl;

	//----------------------------------------------
	//TUTAJ DODAWAC OPCJE DO TWORZENIA ELEMENTOW !!!
	//----------------------------------------------

	if(name == "Dimental") current = new Dimental;
	else if(name == "GeneralMenu") current = new GeneralMenu;
	else if(name == "Authors") current = new Authors;
	else if(name == "Scene") current = new Scene;
	//else if(name == "Editor") current = new StageEditor;
	else return false;

	//----------------------------------------------
	//----------------------------------------------

	current->load(desc[id].location);
	current->sharedElement = desc[id].shared;
	current->nextElement = desc[id].next;

	//cout<<"Zaladowano!\n";

	start=(float)SDL_GetTicks();
	delta=0;

	return true;
}

/**********************************************************************
 METODA: PRYWATNA, STATYCZNA
 Wczytuje obiekty wspoldzielone. Istnieja one przez
**********************************************************************/
void GameEngine::loadSharedObjects()
{
	Object3D *tmp = NULL;

	for(unsigned int i=0; i<desc.size(); i++)
	{
		if(!desc[i].shared) continue;

		if(desc[i].name == "Dimental") tmp = new Dimental;
		else if(desc[i].name == "GeneralMenu") tmp = new GeneralMenu;
		else if(desc[i].name == "Authors") tmp = new Authors;
		else if(desc[i].name == "Scene") tmp = new Scene;
		//else if(desc[i].name == "Editor") tmp = new StageEditor;
		else { desc.erase(desc.begin()+i); i--; continue; }

		tmp->load(desc[i].location);
		tmp->nextElement = desc[i].next;
		tmp->sharedElement = true;

		shared.push_back(tmp);

		tmp = NULL;
	}
}

/**********************************************************************
 METODA: PRYWATNA, STATYCZNA
 Wczytuje opis elementow gry.
**********************************************************************/
void GameEngine::loadDesc(std::string fileName)
{
	ifstream file(fileName.c_str());
	string buffer;
	Desc d;

	//Pominiecie pierwszych trzech linii pliku.
	while(file.get()!='\n'); while(file.get()!='\n'); while(file.get()!='\n');

	while(!file.eof())
	{
		file>>d.name>>d.location>>d.next;
		file>>buffer;
		if(buffer == "true") d.shared = true; else d.shared = false;
		//cout<<d.shared<<endl;
		desc.push_back(d);
	}

	file.close();
}

/**********************************************************************
 METODA: PUBLICZNA, STATYCZNA
 Inicjalizuje silnik gry.
**********************************************************************/
void GameEngine::init()
{
	//Inicjalizacja SDL i atrybutow OpenGL.
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	const SDL_VideoInfo *vi = SDL_GetVideoInfo();
	scrWidth  = vi->current_w;
	scrHeight = vi->current_h;
	scrWidth  = 800;
	scrHeight = 600;
	SDL_SetVideoMode(scrWidth, scrHeight, 32, SDL_HWSURFACE|SDL_OPENGL);
	//SDL_SetVideoMode(scrWidth, scrHeight, 32, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		const char *error = (const char*) glewGetErrorString(err);
		//std::string error(glewGetErrorString(err));
		std::string s(error);
	}

	//if(GLEW_ARB_vertex_buffer_object)
	{
		GLuint vbo = 0;
		glGenBuffersARB(1, &vbo);
	}

	glEnable(GL_CULL_FACE);

	StringResources::loadFonts("GeneralMenu");

	loadDesc("desc.txt");

	//Inicjalizacja klas.
	Object3D::init(&delta);
	CGShader::init();
	SoundManager::init();
	TextureManager::setLocation("Textures");
	CollisionDetector::init();

	SoundManager::addSound("menu.wav","menu");
	SoundManager::addSound("game.wav","game");

	loadSharedObjects();
	lemur.load("Lemur");

	lemur.setSuit("Clothes/Pirate");
	layout.load("GUI");
	layout.ustawCzas(1,20);
	layout.addItems("pow1.tga");
	layout.setStageName("Dzungla");
	layout.setScore(250);
	layout.setLife(3);

	//Stworzenie pierwszego elementu.
	getElement(desc[0].name);
	//getElement("Scene");

	initialized = true;
}

/**********************************************************************
 METODA: PUBLICZNA, STATYCZNA
 Niszczy komputer, palac dysk twardy i pamiec ram. Potem powoduje
 pozar i zabija na miejsu uzytkownika.
 Osobom o slabych nerwach radzimy nie uzywanie tej metody.
 Metoda ta powoduje raka pluc i choroby serca.
 Osoby chore na epilepsje moga zginac na miejscu.
**********************************************************************/
void GameEngine::run()
{
	if(!initialized) return;

	start = (float)SDL_GetTicks();
	while(true)
	{
		EventManager::getEvents();
		//if(EventManager::eventOccured(SDL_QUIT) || EventManager::keyDown(SDLK_ESCAPE)) break;

		end=(float)SDL_GetTicks();
		delta=float(end-start)/1000.0f;
		start=end;

		//Biezacy element zakonczyl dzialanie.
		if(!current->isActive() && !getElement(current->nextElement)) break;

		current->update();
		current->draw();

		SDL_GL_SwapBuffers();
	}
}

void GameEngine::cleanup()
{
	if(current && !current->sharedElement) {delete current; current=NULL;}

	TextureManager::clean();
	CGShader::release();
	CollisionDetector::release();

	SDL_Quit();
}
