#include "SceneController.h"
#include"Scene.h"
void SceneController::ResetScene(std::shared_ptr<Scene> newScene)
{
	scenes_.clear();
	scenes_.push_back(newScene);
}
void SceneController::ChangeScene(std::shared_ptr<Scene> newScene)
{
	if (scenes_.empty()) {//empty()はリストやvectorが空っぽであることを示します
		scenes_.push_back(newScene);
		return;
	}
	//back()はリストの末尾の要素を示している
	scenes_.back() = newScene;//現在使用してるシーンを入れ替える
}

void SceneController::PushScene(std::shared_ptr<Scene> newScene)
{
	scenes_.push_back(newScene);
}

void SceneController::PopScene()
{
	if (scenes_.size() <= 1) {
		return;
	}
	//push_backされたシーンを削除する
	scenes_.pop_back();
}

void SceneController::Update(Input& input)
{
	//今一番手前にあるシーンのUpdateを呼び出す
	//一番手前以外のUpdateは呼び出されない(状態が変わらない)
	scenes_.back()->Update(input);
}

void SceneController::Draw()
{
	//全部Drawする(ポーズしたときも、ゲームの画面が表示されておいてほしいため
	//一番手前だけではなく、すべて描画する
	//ただし、手前の描画が最後に来るように並べておく
	for (auto& scene : scenes_) {
		scene->Draw();
	}
	
}
