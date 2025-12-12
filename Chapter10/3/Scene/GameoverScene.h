#pragma once
#include "Scene.h"
class GameoverScene : public Scene
{
public:
	GameoverScene(SceneController& controller);
	void Update(Input&)override;
	void Draw()override;
};

