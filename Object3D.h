#ifndef OBJECT3D_H_
#define OBJECT3D_H_



#include <SDL_OpenGL.h>
#include "Object.h"
#include "Mathematics.h"

class Object3D: public Object
{
	public:
		Object3D();
		virtual ~Object3D();

		virtual void update() {}
		virtual void draw() {}
		virtual void load(std::string location) {};
		virtual void reset() {}
		virtual void rotateY(float angle) {};

		void setFront(Vec3& f);
		void setUp(Vec3& u);
		void setRight(Vec3& r);
		virtual void setPosition(Vec3& p);
		virtual void setPosition(float x, float y, float z) {position.xyz(x,y,z);}

		Vec3& getFront();
		Vec3& getUp();
		Vec3& getRight();
		Vec3& getPosition();

		static void init(float *delta);
		static void release();

		std::string elementName;	//Nazwa elementu.
		std::string nextElement;	//Nazwa elementu nastepnego.
		bool sharedElement;			//Element jest wspoldzielony.
		virtual std::string getElementName() {return elementName;}

	protected:
		Vec3 position, front, right, up;

		static float *delta;
		static bool initialized;
};

#endif /*OBJECT3D_H_*/
