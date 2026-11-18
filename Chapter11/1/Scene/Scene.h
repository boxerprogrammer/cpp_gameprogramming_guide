#pragma once

//プロトタイプ宣言
//ここで「クラスの名前」だけ宣言する
//中身に関しては不明でいい
//プロトタイプ宣言でなんとかなるのは、参照もしくはポインタのみです
class SceneController;
class Input;

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
protected:
	SceneController& controller_;//シーンコントローラの参照
public:
	/// <summary>
	/// コンストラクタでシーンコントローラを受け取れるように
	/// </summary>
	/// <param name="ctrl">シーンコントローラへの参照</param>
	Scene(SceneController& ctrl);

	/// <summary>
	/// シーンを更新(状態や座標やその他数字的なものを現在の状況に合わせて更新)
	/// </summary>
	/// <param name="input">ユーザーからの入力</param>
	virtual void Update(Input& input)=0;//純粋仮想関数

	/// <summary>
	/// シーンを描画
	/// </summary>
	virtual void Draw() = 0;//純粋仮想関数
};

