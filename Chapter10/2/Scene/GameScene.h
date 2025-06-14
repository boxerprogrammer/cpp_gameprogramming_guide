#pragma once
#include "Scene.h"
#include<vector>
#include<string>
class GameScene :
    public Scene
{
private:
	int bgH_;
	int frame_ = 0;

	int yureFrame_;
	float yureRate_=0.0f;
	int RT_;//レンダーターゲット
	int RTBloom_;//光らすRT
	int RTSmall_;//縮小バッファ用RT
	int ps_;//ピクセルシェーダ

	using UpdateFunc_t = void (GameScene::*)(Input& input);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpdate(Input& input);//フェードイン
	void NormalUpdate(Input& input);//通常
	void FadeOutUpdate(Input& input);//フェードアウト

	void NormalDraw();//通常描画
	void FadeDraw();//フェード描画

	struct Vector3 {
		float x, y, z;
	};
	struct Location {
		std::string name;//メッシュ名
		Vector3 pos;//場所
		Vector3 angle;//回転
	};
	std::vector<Location> locationData_;
	void LoadLocationData();//配置データを読み込む

	std::vector<int> models_;

	Vector3 cameraPos_;

public:
	/// <summary>
	/// シーン初期化(の際にシーンコントローラも代入)
	/// </summary>
	/// <param name="controller"></param>
	GameScene(SceneController& controller);
	~GameScene();
	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	void Update(Input& input) override;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw() override;
};

