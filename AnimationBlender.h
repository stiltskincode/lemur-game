#pragma once

#include "AnimationSet.h"

class AnimationBlender
{
	public:
		AnimationBlender();
		virtual ~AnimationBlender();
		void reset();
		bool blend(AnimationSet *a, AnimationSet *b, AnimationSet *c, float duration, bool blendBack=false);
		void update(float delta);

		AnimationSet *a,*b,*c;
		float elapsedTime, duration;
		bool done, blendBack;
};
