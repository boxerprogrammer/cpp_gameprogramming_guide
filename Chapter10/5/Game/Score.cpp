#include "Score.h"

void Score::Add(uint64_t score)
{
    currentScore_ += score;
}

void Score::AddGraze(uint32_t graze)
{
    graze_ += graze;
}

uint64_t Score::GetCurrent() const
{
	return currentScore_;
}

uint32_t Score::GetGraze() const
{
	return graze_;
}
