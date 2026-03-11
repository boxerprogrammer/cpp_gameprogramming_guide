#pragma once
#include "Scene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene//Sceneから継承したTitleScene
{
private:
	struct ConstantBuffer {//定数バッファ
		float threshold;
		float lx,ly,lz;
	};
	ConstantBuffer* pCBuff_;//GPUに転送するためのCPUのアドレス
	int cBuffH_;//GPU上のコンスタントバッファのハンドル

	int psH_;//ピクセルシェーダハンドル
	int psNormH_;//法線マップピクセルシェーダハンドル

	int titleH_;//タイトル画像ハンドル
	int titleLogoH_;//タイトルロゴハンドル
	int fadeFrame_;//経過フレーム
	int frame_=0;
	int startSE_ = 0;

	int effResHandle_ = -1;
	int effPlayHandle_ = -1;

	//メンバ関数ポインタを使用して内部状態を書き換える
	//update関数とdraw関数を切り替える
	void FadeInUpdate(Input&);//フェードインUpdate
	void NormalUpdate(Input&);//通常Update
	void FadeOutUpdate(Input&);//フェードアウトUpdate

	void (TitleScene::* func_)(Input&);

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	UpdateFunc_t update_;//Update関数を代入できるメンバ関数ポインタ


	void NormalDraw();//通常Draw
	void FadeDraw();//フェードDraw
	using DrawFunc_t = void(TitleScene::*)();
	DrawFunc_t draw_;//Draw関数を代入できるメンバ関数ポインタ


	void DrawShadowedString(int x, int y ,const wchar_t* wstr, unsigned int color = 0xffffff);

public:
	TitleScene(SceneController&);
	~TitleScene();
	void Update(Input&)override;
	void Draw()override;
};

