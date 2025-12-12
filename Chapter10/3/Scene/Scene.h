#pragma once
class SceneController;//シーンコントローラのプロトタイプ宣言
class Input;//入力のプロトタイプ宣言
/// <summary>
/// シーン基底クラス
/// </summary>
class Scene
{
protected:
	//シーンコントローラの参照を持っている
	//ただ単に、それぞれの派生クラスでいちいち新しく
	//宣言するのが面倒だから
	SceneController& controller_;//controller_はメンバ変数
public:
	Scene(SceneController& controller);
	/// <summary>
	/// シーンの情報の更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	/// シーンの描画(シーンの持ち物も描画する)
	/// </summary>
	virtual void Draw() = 0;
};

