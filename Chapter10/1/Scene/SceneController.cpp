#include "SceneController.h"
#include"Scene.h"

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	if (scenes_.empty()) {
		scenes_.push_back(scene);
	}
	else {
		scenes_.back() = scene;
	}
	
	//�����Ascene_�ɂ��łɒl�������Ă����ꍇ��
	//shared_ptr�Ȃ̂ŁA���Ƃ�scene_�̎Q�Ɛl����0��
	//�Ȃ�A�����ŉ������܂��B
	//�����Ashared_ptr���g���ĂȂ��ꍇ
}

void SceneController::Update(Input& input)
{
	scenes_.back()->Update(input);
}

void SceneController::Draw()
{
	for (auto& scene : scenes_) {
		scene->Draw();
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneController::PopScene()
{
	if (scenes_.size() > 1) {
		scenes_.pop_back();
	}
}

void SceneController::JumpScene(std::shared_ptr<Scene> scene)
{
	scenes_.clear();
	scenes_.push_back(scene);
}
