#pragma once

#include "Character.h"
#include "Stage.h"
#include <ctime>
#include <cstdlib>
#include <sstream>

class Monster : public Character
{
	public:
		Monster();
		virtual ~Monster();

		void load(std::string location);
		void init(Stage *stage);
		void rotateLeft();
		void rotateRight();
		void handleInput();
		virtual void setPosition(float x, float y, float z);

	protected:
		Stage *stage;
		int stageX, stageZ;
		float sumRotationAngle;
		float step;
		float correctMatrix;
		bool reverse;
		bool occupyField;
		Uint8 newkeystate[11];
};
