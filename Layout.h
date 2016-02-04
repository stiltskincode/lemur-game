#ifndef LAYOUT_H_
#define LAYOUT_H_


#include <GL/glew.h>
#include <SDL_OpenGL.h>
#include "TGAImage.h"
#include <vector>
#include <deque>
#include "Object3D.h"
#include "BitmapFont.h"
#include "BitmapString.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "Stopwatch.h"

class Layout;

//struktura swp wierzchalka tekstury
struct Corner
{
	float x;
	float y;
};


//struktura calego itemu
struct Boombs
{
	bool active;
	bool czasowy;
	Corner a,
		   b,
		   c,
		   d;
	GLuint itemType;
	Stopwatch watch;
	int wartosc;
};



class Layout : public Object3D
{
public:
	GLuint  postac, item, timex, stagex, punktyx, zyciax, bombax;
	GLuint bomby[3];
	float rotatex;
	BitmapFont font;
	BitmapString bitmaps;
	std::deque<Boombs>boombs;
	Boombs b;
	std::vector<GLuint> items;



	Stopwatch watch;


	Layout();
	//laduje tekstury i inne rzeczy
	void load(std::string location);

	//wyswietla wszystko
	void set(int, int);

	//dodaje item do wektora

	//ustawia itemu dla planszy
	void rotateBombLeft();
	void rotateBombRight();



	void addItems(const char*);
	void setItems(int item, float czas, int wartosc);
	void setItems(int item, int wartosc);
	void displayItems();
	void transformItems(void);
	void setStageName(string);
	void setScore(int);
	void setLife(int);
	void ustawCzas(float, float);

	private:
		float bomb_tmp, rotate_tmp, last_rotate_tmp;
		bool rotateLeft, rotateRight;
		string stageName;
		int  score , life;
		Vec3 bombPoints[3], finalBombPoints[3];

		int widnowWidth, windowHeigth;
		void window3d();
		void czas();
		void stage();
		void punkty();
		void zycia();
		void bronie();
		void portret();
		void bomb();
		void transact();


		void setTime(void);

		void setString();

		void setBanner(void);
		void transform(void);
};

#endif
