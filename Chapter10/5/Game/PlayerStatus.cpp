#include "PlayerStatus.h"
#include "PlayerStatus.h"

void PlayerStatus::LoseLife()
{}

bool PlayerStatus::UseBomb()
{
    return false;
}

void PlayerStatus::ExtendLife()
{}

void PlayerStatus::AddBomb()
{}

int PlayerStatus::GetLives() const
{
    return 0;
}

int PlayerStatus::GetBombs() const
{
    return 0;
}

bool PlayerStatus::IsGameOver() const
{
    return false;
}

void PlayerStatus::AddPlayerLevel()
{

}

void PlayerStatus::ResetPlayerLevel()
{}
