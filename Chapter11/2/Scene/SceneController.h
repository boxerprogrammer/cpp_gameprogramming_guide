#pragma once
#include<memory>
#include<list>
class Scene;
class Input;
/// <summary>
/// シーン遷移に必要な関数を提供
/// 現在のシーンを更新・描画する
/// </summary>
class SceneController
{
private:
	std::list<std::shared_ptr<Scene>> scenes_;//シーンは複数ある可能性がある(通常は１つでいい)
public:

	/// <summary>
	/// シーンが１つだけ積まれてる状態に戻す
	/// </summary>
	/// <param name="newScene">唯一のシーン</param>
	void ResetScene(std::shared_ptr<Scene> newScene);

	/// <summary>
	/// シーンの切り替えを行う。シーンがない場合は新規にシーンがセットされます
	/// </summary>
	/// <param name="newScene">新しいシーンオブジェクト</param>
	void ChangeScene(std::shared_ptr<Scene> newScene);
	
	/// <summary>
	/// 新しいシーンを押し込む
	/// </summary>
	/// <param name="newScene">新しいシーン</param>
	void PushScene(std::shared_ptr<Scene> newScene);

	/// <summary>
	/// 最後のシーンを外す(ただしシーンが１つしかない時は無効)
	/// </summary>
	void PopScene();

	/// <summary>
	/// 現在のシーンの状態を更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(Input& input);

	/// <summary>
	/// 持っているシーンを描画
	/// </summary>
	void Draw();
};

