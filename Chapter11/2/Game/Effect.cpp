#include "Effect.h"
#include "Effect.h"

Effect::Effect(const Position2& pos, int maxFrame,int handle):
	Actor(pos,0.0f),
	maxFrame_(maxFrame),
	handle_(handle)
{
}

void Effect::Update()
{
	if (maxFrame_ <= ++frame_) {
		isDead_ = true;
	}
}
