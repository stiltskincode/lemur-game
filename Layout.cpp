#include "Layout.h"

Layout::Layout()
{
	bombPoints[0].xyz(0,0,50);
	bombPoints[1]=bombPoints[2]=bombPoints[0];
	bombPoints[1].rotate(0,120,0);
	bombPoints[2].rotate(0,-120,0);

	finalBombPoints[0] = bombPoints[0];
	finalBombPoints[1] = bombPoints[1];
	finalBombPoints[2] = bombPoints[2];
}

void Layout::ustawCzas(float m, float s)
{
		watch.minutes = m;
		watch.secounds = s;
}


void Layout::load(std::string location)
{
	timex = TGAImage::loadGLTexture(location+"/time.tga");
	punktyx = TGAImage::loadGLTexture(location+"/punkty.tga");
	stagex = TGAImage::loadGLTexture(location+"/nazwa.tga");
	zyciax = TGAImage::loadGLTexture(location+"/zycia.tga");
	bombax = TGAImage::loadGLTexture(location+"/bomba.tga");
	postac = TGAImage::loadGLTexture(location+"/portret.tga");
	rotateLeft = false;
	rotateRight = false;

	font.create("euro.ini","euro.tga");
	bitmaps.setFont(&font);

	bomby[0] = TGAImage::loadGLTexture(location+"/bomba.tga");
	bomby[1] = TGAImage::loadGLTexture(location+"/bomba.tga");
	bomby[2] = TGAImage::loadGLTexture(location+"/bomba.tga");
}

void Layout::addItems(const char* name)
{
	//item = TGAImage::loadGLTexture("GUI/"+string(name));
	item = TGAImage::loadGLTexture(name);
	items.push_back(item);
}



void Layout::setItems(int item, float czas, int wartosc)
{
	int x = boombs.size();
	if(x<7)
	{
		Boombs b_tmp;
		b_tmp.active = true;
		b_tmp.a.x = - 0.0625*widnowWidth;
		b_tmp.b.x =  - 0.0625*widnowWidth;
		b_tmp.c.x = 0.0;
		b_tmp.d.x = 0.0;

		b_tmp.a.y =  0.7083 * windowHeigth - 0.1 * windowHeigth *x;
		b_tmp.b.y =  0.7916 * windowHeigth - 0.1 * windowHeigth *x;
		b_tmp.c.y =  0.7916 * windowHeigth - 0.1 * windowHeigth *x;
		b_tmp.d.y =  0.7083 * windowHeigth -  0.1 * windowHeigth*x;
		b_tmp.itemType = items[item];
		b_tmp.watch.setTime(czas);
		//b_tmp.watch.secounds = czas;
		b_tmp.wartosc = wartosc;
		boombs.push_back(b_tmp);
	}
}

void Layout::transformItems()
{
	for (unsigned i=0; i<boombs.size();i++)
	{
		/*if (boombs[i].watch.secounds>0)
			boombs[i].time.secound = boombs[i].time.secound - *delta;

		if(boombs[i].time.secound<0.0)
			boombs[i].time.secound = 0.0;

		*/
		boombs[i].watch.start();

		if(boombs[i].watch.secounds<=0.0 && boombs[i].active==true)
			boombs[i].active=false;
		//std::cout<<boombs[i].watch.minutes<<" : "<<boombs[i].watch.secounds<<std::endl;;

		if(!boombs[i].active)
		{
			boombs[i].a.x -=0.1;
			boombs[i].b.x -=0.1;
			boombs[i].c.x -=0.1;
			boombs[i].d.x -=0.1;
		}

		if(boombs[i].active && boombs[i].c.x <50.0)
		{
			boombs[i].a.x +=0.1;
			boombs[i].b.x +=0.1;
			boombs[i].c.x +=0.1;
			boombs[i].d.x +=0.1;
		}

		if(!boombs[i].active && boombs[i].c.x <=0.0)
		{
			boombs.erase(boombs.begin()+i);
//			for (unsigned n = i; n<boombs.size();n++)
//			{
//				boombs[n] = boombs[n+1];
//			}
//			boombs.pop_back();
		}

		if(boombs[i].active && boombs[i].c.x>=50.0)
		{
			if(boombs[i].c.y < 0.7916 * windowHeigth - 0.1 * windowHeigth *i)
				{
				boombs[i].a.y+=0.1;
				boombs[i].b.y+=0.1;
				boombs[i].c.y+=0.1;
				boombs[i].d.y+=0.1;

				}
		}
	}
}


void Layout::displayItems(void)
{
	transformItems();
	glEnable(GL_TEXTURE_2D);
	char buffer [2];
	for (unsigned i=0; i<boombs.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, boombs[i].itemType);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0);
			glVertex3f(boombs[i].a.x,boombs[i].a.y,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(boombs[i].b.x,boombs[i].b.y,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(boombs[i].c.x,boombs[i].c.y,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(boombs[i].d.x,boombs[i].d.y,0);
		glEnd();

		itoa ((int)boombs[i].watch.secounds,buffer,10);

		glPushMatrix();
	 	glTranslatef(boombs[i].d.x,boombs[i].d.y+5, 0.0 );
	 	glScalef(0.4,0.4,0.4);
	 	glColor3f(0,0,0);
	 	glColor3f(1,0,1);
	 	bitmaps.set(buffer);
	 	//bitmaps.setCentered(true);
		bitmaps.draw(0);

		glColor3f(1,1,1);

		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		itoa ((int)boombs[i].wartosc,buffer,10);
 		glTranslatef(boombs[i].c.x,boombs[i].c.y-20, 0.0 );
 		glScalef(0.4,0.4,0.4);
 		glColor3f(0,1,0);

 		bitmaps.set(buffer);
 		//bitmaps.setCentered(true);
 	    bitmaps.draw(0);

 		glColor3f(1,1,1);
 		glPopMatrix();
	    glEnable(GL_TEXTURE_2D);
	}
glColor3f(1,1,1);

}




void Layout::czas()
{
	/*
	  glBindTexture(GL_TEXTURE_2D,timex);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.62*widnowWidth,0.9216*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.62*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.73*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.73*widnowWidth,0.9216*windowHeigth,0);
		glEnd();

		*/

		  glBindTexture(GL_TEXTURE_2D,timex);
		    glBegin(GL_QUADS);
		    	glTexCoord2f(0.0,0.0);
				glVertex3f(widnowWidth-0.146*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
				glTexCoord2f(0.0,1.0);
				glVertex3f(widnowWidth-0.146*windowHeigth,windowHeigth,0);
				glTexCoord2f(1.0,1.0);
				glVertex3f(widnowWidth,windowHeigth,0);
				glTexCoord2f(1.0,0.0);
				glVertex3f(widnowWidth,windowHeigth - (0.064*windowHeigth),0);
			glEnd();




}

void Layout::stage()
{
	/*
	    glBindTexture(GL_TEXTURE_2D,stagex);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.32*widnowWidth,0.9216*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.32*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.60*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.60*widnowWidth,0.9216*windowHeigth,0);
		glEnd();

		*/


	    glBindTexture(GL_TEXTURE_2D,stagex);
		    glBegin(GL_QUADS);
		    	glTexCoord2f(0.0,0.0);
		    	glVertex3f(((widnowWidth + 0.327*windowHeigth)/2)-0.192*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
		    	glTexCoord2f(0.0,1.0);
		    	glVertex3f(((widnowWidth + 0.327*windowHeigth)/2)-0.192*windowHeigth,windowHeigth,0);
		    	glTexCoord2f(1.0,1.0);
		    	glVertex3f(((widnowWidth + 0.327*windowHeigth)/2)+0.192*windowHeigth,windowHeigth,0);
		    	glTexCoord2f(1.0,0.0);
		    	glVertex3f(((widnowWidth + 0.327*windowHeigth)/2)+0.192*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
			glEnd();

}
void Layout::punkty()
{
	/*
	    glBindTexture(GL_TEXTURE_2D,punktyx);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.165*widnowWidth,0.9216*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.165*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.3*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.3*widnowWidth,0.9216*windowHeigth,0);
		glEnd();

		*/


	    glBindTexture(GL_TEXTURE_2D,punktyx);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.28*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.28*windowHeigth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.28*windowHeigth+0.193*windowHeigth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.28*windowHeigth+0.193*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
		glEnd();



}
void Layout::zycia()
{
	  /*  glBindTexture(GL_TEXTURE_2D,zyciax);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.1125*widnowWidth,0.9216*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.1125*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.1625*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.1625*widnowWidth,0.9216*windowHeigth,0);
		glEnd();

		*/

	    glBindTexture(GL_TEXTURE_2D,zyciax);
	    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0.19*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0.19*windowHeigth,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.19*windowHeigth+0.076*windowHeigth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.19*windowHeigth+0.076*windowHeigth,windowHeigth - (0.064*windowHeigth),0);
		glEnd();


}
void Layout::portret()
{
	/*	glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D,postac);
		    glColor3f(1,1,1);
		    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0,0.8*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.1225*widnowWidth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.1225*widnowWidth,0.8*windowHeigth,0);
		glEnd();
		*/


		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D,postac);
		    glColor3f(1,1,1);
		    glBegin(GL_QUADS);
	    	glTexCoord2f(0.0,0.0);
			glVertex3f(0,windowHeigth - 0.2*windowHeigth,0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(0,windowHeigth,0);
			glTexCoord2f(1.0,1.0);
			glVertex3f(0.2*windowHeigth,windowHeigth,0);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.2*windowHeigth,windowHeigth - 0.2*windowHeigth,0);
		glEnd();








}

void Layout::setStageName(string name)
{
	this->stageName = name;
}

void Layout::setScore(int score)
{
	this->score = score;
}

void Layout::setLife(int life)
{
	this->life = life;
}

void Layout::setString()
{
	char buffer [10];
	glPushMatrix();
	glLoadIdentity();
	std::string godzina;
	 if (watch.minutes<10)
         godzina +="0";
	 itoa ((int)watch.minutes,buffer,10);
	 godzina +=buffer;
	 godzina +=":";
	 if (watch.secounds<10)
	   	 godzina +="0";
	 itoa ((int)watch.secounds,buffer,10);
	  	 bitmaps.set(buffer);
	  	godzina +=buffer;
	  	glTranslatef((widnowWidth-0.073*windowHeigth),windowHeigth - (0.030*windowHeigth),0);
	  	  glScalef(0.5,0.5,0.5);




	 bitmaps.set(godzina);
	 bitmaps.setCentered(true);
	 bitmaps.draw(0);
	 glPopMatrix();


	//  itoa ((int)watch.minutes,buffer,10);
       // godzina +=buffer;
       // godzina +=" : ";

  	  //if (watch.secounds<10)
  	  	//godzina +="0";

  //	 itoa ((int)watch.secounds,buffer,10);
  		//  bitmaps.set(buffer);

  		//  godzina +=buffer;


  		// glPushMatrix();
  		// glLoadIdentity();

  		//glTranslatef((0.675*widnowWidth)-(0.0375*widnowWidth),windowHeigth - (0.0392*windowHeigth),0);
  		 //glScalef(0.5,0.5,0.5);
  		// bitmaps.set(godzina);





	  glLoadIdentity();

	  bitmaps.set(this->stageName);
	 // glTranslatef(0.46*widnowWidth  /* - (bitmaps.width/2)*/,0.94*windowHeigth,0);
	  glTranslatef((widnowWidth + 0.327*windowHeigth)/2,windowHeigth - (0.030*windowHeigth),0);
	  glScalef(0.5,0.5,0.5);

	  bitmaps.setCentered(true);
	  bitmaps.draw(0);

	  glPopMatrix();

	  glPushMatrix();
	 glLoadIdentity();
	  glTranslatef(0.3765*windowHeigth,windowHeigth - (0.030*windowHeigth),0);
	  glScalef(0.5,0.5,0.5);

	 itoa ((int)this->score,buffer,10);
	  bitmaps.set(buffer);
	  bitmaps.setCentered(true);
	  bitmaps.draw(0);

	  glPopMatrix();

	  glPushMatrix();
	  glLoadIdentity();

	//zycia
	  glTranslatef(0.228*windowHeigth,windowHeigth - (0.03*windowHeigth),0);
	 glScalef(0.5,0.5,0.5);
	 itoa ((int)this->life,buffer,10);
	  bitmaps.set(buffer);
	  bitmaps.setCentered(true);
	  bitmaps.draw(0);

	 glPopMatrix();
	 glPopMatrix();
}


void Layout::rotateBombLeft()
{
	rotateLeft = true;
	rotateRight = false;

}
void Layout::rotateBombRight()
{
	rotateRight = true;
	rotateLeft = false;
}

void Layout::bomb()
{
	if(rotateLeft)
	{
		if(rotate_tmp<120 )
		{
			for(int i=0; i<3; i++) bombPoints[i].rotate(0,(*delta)*240,0);
			rotate_tmp+=(*delta)*240;
		}
		else
		{
			rotate_tmp = 0.0;
			rotateLeft = false;

			//Korekcja pozycji.
			for(int i=0; i<3; i++)
			{
				if(bombPoints[i][2]>0)
				{
					bombPoints[i] = finalBombPoints[0];
					for(int j=0; j<3; j++)
					{
						if(j==i) continue;
						if(bombPoints[j][0]<0) bombPoints[j] = finalBombPoints[2];
						else bombPoints[j] = finalBombPoints[1];
					}
				}
			}
		}
	}

	if (rotateRight)
	{
		if(rotate_tmp>-120 )
		{
			for(int i=0; i<3; i++) bombPoints[i].rotate(0,-(*delta)*240,0);
			rotate_tmp-=(*delta)*240;
		}
		else
		{
			rotate_tmp = 0.0;
			rotateRight = false;

			//Korekcja pozycji.
			for(int i=0; i<3; i++)
			{
				if(bombPoints[i][2]>0)
				{
					bombPoints[i] = finalBombPoints[0];
					for(int j=0; j<3; j++)
					{
						if(j==i) continue;
						if(bombPoints[j][0]<0) bombPoints[j] = finalBombPoints[2];
						else bombPoints[j] = finalBombPoints[1];
					}
				}
			}
		}
	}
}

void Layout::bronie()
{
	   glViewport((widnowWidth/2)-(windowHeigth/3), 0, windowHeigth/1.5,windowHeigth/6);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(20,8.0f/3.0f,1,10000);
	    glEnable(GL_TEXTURE_2D);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    gluLookAt(0,25,350,0,25,0,0,1,0);
	    glEnable(GL_DEPTH_TEST);
	   glBindTexture(GL_TEXTURE_2D,bombax);
        glPushMatrix();
		glTranslatef(0,0,0);


		bomb();

		for(int i=0; i<3; i++)
		{
			glPushMatrix();
				glTranslatef(bombPoints[i][0], bombPoints[i][1], bombPoints[i][2]);
				float z = 0.7f + (bombPoints[i][2]/100.0f);
				glColor3f(z,z,z);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0,0.0);
					glVertex3f(-20,0,100);
					glTexCoord2f(0.0,1.0);
					glVertex3f(-20,60,100);
					glTexCoord2f(1.0,1.0);
					glVertex3f(20,60,100);
					glTexCoord2f(1.0,0.0);
					glVertex3f(20,0,100);
				glEnd();
			glPopMatrix();
		}

//		for(int i=0; i<3; i++)
//		{
//			glPushMatrix();
//			glRotatef(i*120,0,1,0);
//			 glBegin(GL_QUADS);
//				 glTexCoord2f(0.0,0.0);
//				 glVertex3f(-20,0,100);
//				 glTexCoord2f(0.0,1.0);
//				 glVertex3f(-20,60,100);
//				 glTexCoord2f(1.0,1.0);
//				 glVertex3f(20,60,100);
//				 glTexCoord2f(1.0,0.0);
//				 glVertex3f(20,0,100);
//			glEnd();
//
//			glPopMatrix();
//		}


glPopMatrix();


	   	glDisable (GL_ALPHA_TEST);
	   	glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
//		glBegin(GL_LINES);
//			glColor3f(1,0,0); glVertex3f(-500,0,0); glVertex3f(500,0,0);
//			glColor3f(0,1,0); glVertex3f(0,-500,0); glVertex3f(0,500,0);
//			glColor3f(0,0,1); glVertex3f(0,0,-500); glVertex3f(0,0,500);
//		glEnd();




}




void Layout::window3d()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); gluPerspective(45,4.0f/3.0f,1,10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(100,50,100,0,0,0,0,1,0);

   // glRotatef(angle,0,1,0);

    glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
		glVertex3f(-20,-20,0);
		glVertex3f( 20,-20,0);
		glVertex3f( 0,  20,0);
    glEnd();
};


void Layout::set(int x, int y)
{
	//ustawiamy rysowanie 2d
	widnowWidth = x; windowHeigth = y;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x,0,y);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //paramtery tekstury
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.5);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1,1,1);

    //rysowanie baneru
   // setBanner();

    watch.start();
    czas();
    stage();
    punkty();
    zycia();

    portret();
    setString();
    displayItems();



    //rysowanie broni





  glViewport(0, 500, 100,100);

    //TLO CZARNE (CZARNY QUAD) rysowany w "2d"


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,100,0,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

bronie();

//std::cout<<clock.minutes<<" : "<<clock.secound<<std::endl;
    //Przechodzimy na macierz "3d"
   // portret();
	//window3d();

};
