#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>

#include "Scene.h"
class Input;
class PauseScene :
    public Scene
{
private:
    int frame_ = 0;
    int currentIndex_ = 0;
    //���j���[�̕�����Ɗ֐��̃e�[�u��
    using MenuFunc_t = std::function<void(Input&)>;
    std::vector<std::wstring> menuList_;
    std::map<std::wstring,MenuFunc_t> menuFuncTable_;

    using UpdateFunc_t = void (PauseScene::*)(Input& input);
    using DrawFunc_t = void (PauseScene::*)();

    UpdateFunc_t update_;//���t���[���X�V�֐��|�C���^
    DrawFunc_t draw_;//���t���[���`��֐��|�C���^

    //�X�V�֐�
    void AppearUpdate(Input& input);//�g���L���ĕ\������֐�
    void NormalUpdate(Input& input);//�g����у��j���[��\������֐�
    void DisappearUpdate(Input& input);//�g���k�����ď����֐�
   
    //�`��֐�
    void ProcessDraw();//�g�̏o���E���Ŏ��̉��o�`��
    void NormalDraw();//�ʏ�̃|�[�Y���j���[�`��

    //���j���[�̃��X�g��\������
    void DrawMenuList();


public:
    PauseScene(SceneController& controller);
    virtual void Update(Input& input)override;
    virtual void Draw()override;

};

