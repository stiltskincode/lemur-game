#ifndef BLOCKADE_H_
#define BLOCKADE_H_

#include "Monster.h"

class Blockade: public Monster
{
	public:
		Blockade();
		virtual ~Blockade();

		virtual void handleInput();
		virtual void load(std::string location);

		virtual void setPosition(float x, float y, float z);
};

#endif /* BLOCKADE_H_ */
