#pragma once
#include<vector>
#include"../Geometry.h"
//背景(多重スクロール機能つき)
class Background
{
private:
	struct BGPart {
		int handle = -1;
		float scrollSpeeds;
		bool flip = false;
		Position2 pos;
	};
	std::vector<BGPart> bgParts_;
public:
	Background();
	void Update();
	void Draw();
};

