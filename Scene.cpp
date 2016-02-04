#include <Scene.h>

Scene::Scene()
{
	elementName = "Scene";
	starting = true;
}

Scene::~Scene()
{

}

void Scene::start()
{
	GameEngine::lemur.forceAction=true;
	GameEngine::lemur.forceWalkForward=true;
	GameEngine::lemur.speed=150;

	float distanceWalked = (stage.startPos - GameEngine::lemur.mesh.getPosition()).length();

	if(distanceWalked>=stage.startDistance)
	{
		GameEngine::lemur.speed=300;
		distanceWalked=stage.startDistance;
		GameEngine::lemur.forceWalkForward=false;
		starting=false;

		CollisionDetector::add(&GameEngine::lemur.mesh.getPosition(),"lemur",30,1,false);
	}

	Vec3 lemurFront = GameEngine::lemur.mesh.getFront() * 500;
	lemurFront.rotate(0,180*(distanceWalked/stage.startDistance),0);
	Vec3 lemurPos = GameEngine::lemur.mesh.getPosition();
	Vec3 camPos = GameEngine::lemur.mesh.getPosition();

	camPos = camPos + lemurFront;

	gluLookAt(camPos[0], 500, camPos[2], lemurPos[0], 150, lemurPos[2], 0, 1, 0);
}

void Scene::load(std::string location)
{
	active = true;

	int w = GameEngine::scrWidth;
	int h = GameEngine::scrHeight;

	SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,float(w)/float(h),1,10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//WCZYTANIE PLANSZY I OBIEKTOW PLANSZY.

	Bomb::init(&stage);
	BasicBomb::init(&cam.getFront());
	Bomb::create("bomb");
	BasicBomb::create("BasicBomb");

	skydome.load("skydome.dim");

	stage.load(location);

	for(unsigned int i=0; i<stage.monsters.size(); i++)
	{
		Stage::ExternResource res = stage.monsters[i];

		monsters.push_back(Monster());
		monsters.back().init(&stage);
		monsters.back().load(res.name);
		monsters.back().setPosition(res.x*200+100,0,res.z*200+100);
		monsters.back().rotateY(res.angle*90);
	}

	for(unsigned int i=0; i<stage.blockades.size(); i++)
	{
		Stage::ExternResource res = stage.blockades[i];

		blockades.push_back(Blockade());
		blockades.back().init(&stage);
		blockades.back().load(res.name);
		blockades.back().setPosition(res.x*200+100,0,res.z*200+100);
		blockades.back().rotateY(res.angle*90);
	}

	//Przemieszczenie lemura na pozycje poczatkowa.
	GameEngine::lemur.mesh.tmat.identity();
	GameEngine::lemur.mesh.setPosition(0,0,0);
	GameEngine::lemur.mesh.getFront().xyz(0,0,-1);
	Mat4 rot; rot.rotationY(stage.startAngle);
	GameEngine::lemur.mesh.tmat*=rot;
	GameEngine::lemur.mesh.getFront().rotate(stage.startAngle,Vec3(0,1,0));
	GameEngine::lemur.mesh.tmat[12] = stage.startPos[0];
	GameEngine::lemur.mesh.tmat[14] = stage.startPos[2];
	GameEngine::lemur.mesh.getPosition()=stage.startPos;

	cam.init(&GameEngine::lemur.mesh.getFront(),&GameEngine::lemur.mesh.getPosition(),&GameEngine::lemur.rotationAngle);

	CollisionDetector::setStage(&stage);

	SoundManager::setLoop(true,"game");
	SoundManager::play("game");
}

void Scene::reset()
{
	active=true;
}

void Scene::update()
{

}

void Scene::draw()
{
	if(EventManager::keyDown(SDLK_ESCAPE)) {active=false; SoundManager::stop("game");}
	if(EventManager::keyDown(SDLK_1)) GameEngine::layout.rotateBombLeft();
	if(EventManager::keyDown(SDLK_2)) GameEngine::layout.rotateBombRight();
	if(EventManager::keyDown(SDLK_8)) GameEngine::layout.setItems(0, 15,10);
	if(EventManager::keyDown(SDLK_9)) GameEngine::layout.setItems(0, 10,10);
	if(EventManager::keyDown(SDLK_0)) GameEngine::layout.setItems(0, 20,10);

	int w = GameEngine::scrWidth;
	int h = GameEngine::scrHeight;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,float(w)/float(h),1,10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(starting) start();
	else cam.update();

	glPushMatrix();
	glTranslatef(cam.getPosition()[0], cam.getPosition()[1], cam.getPosition()[2]);
	glDepthMask(GL_FALSE);
	glColor3f(1,1,1);
	skydome.draw();
	glDepthMask(GL_TRUE);
	glPopMatrix();

	CollisionDetector::detect();

	for(unsigned int i=0; i<monsters.size(); i++)
	{
		if(!starting) monsters[i].update();
		monsters[i].draw();
	}
	for(unsigned int i=0; i<blockades.size(); i++)
	{
		if(!starting) blockades[i].update();
		blockades[i].draw();
	}
	GameEngine::lemur.draw();
	GameEngine::lemur.update();

	glEnable(GL_ALPHA_TEST);
	stage.draw();
	glDisable(GL_ALPHA_TEST);

	Bomb::drawBombs();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	GameEngine::layout.set(GameEngine::scrWidth, GameEngine::scrHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
