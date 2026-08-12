#include "PlayerStatus.h"
#include "PlayerStatus.h"

void PlayerStatus::LoseLife()
{
	--lives_;
	bombs_ = initial_bombs;
}

void PlayerStatus::UseBomb()
{
    --bombs_;
}

void PlayerStatus::ExtendLife()
{
	if (lives_ < max_lives) {
		++lives_;
	}
}

void PlayerStatus::AddBomb()
{
	if (bombs_ < max_bombs) {
		++bombs_;
	}
}

int PlayerStatus::GetLives() const
{
    return lives_;
}

int PlayerStatus::GetBombsCount() const
{
    return bombs_;
}

bool PlayerStatus::IsGameOver() const
{
    return lives_ <= 0;
}

void PlayerStatus::AddPlayerLevel()
{

}

void PlayerStatus::ResetPlayerLevel()
{}
