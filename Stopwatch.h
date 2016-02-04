#include "Object3D.h"
class Stopwatch : public Object3D
{
public:
	float hours;
	float minutes;
	float secounds;
	bool startWatch;

	Stopwatch(){};
	~Stopwatch(){};

	inline bool setTime(float, float, float);
	inline bool setTime(float, float);
	inline bool setTime(float);
	inline void start(void);



};


bool Stopwatch::setTime(float secounds)
{
	this->startWatch = true;
	this->hours = 0;
	this->minutes = 0;
	this->secounds = secounds;
	return true;
}

bool Stopwatch::setTime(float minutes, float secounds)
{
	this->startWatch = true;
	this->hours = 0;
	this->minutes = minutes;
	this->secounds = secounds;
	return true;
}


bool Stopwatch::setTime(float hours, float minutes, float secounds)
{
	this->startWatch = true;
	this->hours = hours;
	this->minutes = minutes;
	this->secounds = secounds;
	return true;
}

void Stopwatch::start()
{

		this->secounds = this->secounds - *delta;
		if(this->secounds < 0)
			{

			this->secounds = 60 - *delta;
			this->minutes = this->minutes - 1;
			}

		if(this->minutes < 0)
			{

			this->minutes = 0.0;
			this->secounds = 0.0;
			}

		/*

		if(this->minutes < 0)
			{

			this->minutes = 60 - 1;
			this->hours = this->hours - 1;
			}

			*/

		/*if(this->hours < 0)
			{
				this->secounds = 0.0;
				this->minutes = 0.0;
				this->hours = 0.0;
			}
			*/

}




