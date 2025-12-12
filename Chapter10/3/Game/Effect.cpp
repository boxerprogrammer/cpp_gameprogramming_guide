#include "Effect.h"

Effect::Effect(const Position2& pos) {
	pos_ = pos;
}

bool 
Effect::IsDead()const {
	return isDead_;
}