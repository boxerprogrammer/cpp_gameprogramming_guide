#pragma once
#include "Scene.h"
#include<unordered_map>
#include<string>
#include<array>
#include<functional>
#include"../Input.h"

class KeyConfigScene :
    public Scene
{
private:
    void DrawInputList();
    std::wstring GetPeriphString(const PeripheralType& type);
    Input& input_;
    int currentIndex_ = 0;//���ݑI�𒆂̃C���f�b�N�X
    
    std::array<char,256> lastKeyState_;//�G�f�B�b�g�p�ŁA�O�ɂǂ̃L�[�������ꂽ����ۑ����Ă���
    std::array<char, 256> currentKeyState_;//�G�f�B�b�g�p

    int currentPadState_;
    int lastPadState_;

    using NameTable_t = std::unordered_map<int, std::wstring>;
    NameTable_t keyboardNameTable_;//�L�[�{�[�h���O�e�[�u��
    NameTable_t padNameTable_;//�p�b�h���O�e�[�u��
    NameTable_t analogNameTable_;//�A�i���O���͖��O�e�[�u��

    using SystemMenuTable_t = std::unordered_map<std::wstring, std::function<void(void)>>;
    SystemMenuTable_t systemMenuTable_;
    std::vector<std::wstring> systemMenuStringList_;

    Input::InputTable_t tempInputTable_;//���̓e�[�u���̃_�~�[�e�[�u��(�X�V�͂�������������)


    using UpdateFunc_t = void (KeyConfigScene::*)(Input& input);
    UpdateFunc_t update_;
    void NormalUpdate(Input& input);//��ҏW�����
    void EdittingUpdate(Input& input);//�ҏW���

    
    void CommitInputTable();
    void ReloadTable();
public:
    KeyConfigScene(SceneController& controller,Input& input);
    
    virtual void Update(Input& input)override;
    virtual void Draw()override;
    
};

