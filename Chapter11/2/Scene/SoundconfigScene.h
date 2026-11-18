#pragma once
#include "Scene.h"
#include<string>
#include<map>
#include<functional>
/// <summary>
/// BGMやSEの音量を調整する
/// </summary>
class SoundconfigScene :
    public Scene
{
private:

    std::map < std::wstring, std::function<void(Input&)>> menuFunction_;
    std::vector<std::wstring> menuItems_;
    int selectedIndex_=0;//現在選択中のアイテム
    enum class SoundType {
        bgm,
        se
    };
    std::array<int, 2> tempVolumes_ = {100,100};

    int frame_ = 0;//演出のためのフレーム時間変数

    using UpdateFunc_t = void (SoundconfigScene::*)(Input&);
    using DrawFunc_t = void (SoundconfigScene::*)();

    UpdateFunc_t update_;
    DrawFunc_t draw_;

    void AppearUpdate(Input&);//出現演出
    void NormalUpdate(Input& input);//操作可能状態
    void DisappearUpdate(Input&);//退場演出

    void NormalDraw();//通常描画
    void TransitionEffectDraw();//演出中描画

    /// <summary>
    /// 影付きの文字列を描画する
    /// </summary>
    /// <param name="str">文字列</param>
    void DrawStringWithShadow(int x, int y, uint32_t textCol, const std::wstring& str);
    

    void ChangeVolume(SoundType type, int changeValue);

    int GetChangeValue(const Input& input);

public:
    SoundconfigScene(SceneController& ctrl);
    void CloseWindow();
    void Update(Input& input)override;
    void Draw()override;
};

