#include "Application.h"

#include"Scene/SceneController.h"
#include"Scene/TitleScene.h"//一番最初のシーンだけはアプリケーションで生成
#include"Input.h"

#include<DxLib.h>
#include"FileLoader.h"
//なんでもいいですが、16:9になるようにしておいてください
//constexpr int window_width = 1280;//16*80
//constexpr int window_height = 720;//9*80
constexpr int window_width = 1024;//16*64
constexpr int window_height = 576;//9*64

std::shared_ptr<FileLoader> 
Application::GetLoader(){
    return loader_;
}
//staticなので、誰でも呼べる
Application& Application::GetInstance()
{
    static Application instance;//このApplication実体はメモリが固定される
    return instance;
}

Application::Application() {
    wsize_ = { window_width,window_height };
    loader_ = std::make_shared<FileLoader>();
}

bool Application::Init()
{

    //初期化
    ChangeWindowMode(true);

    SetGraphMode(wsize_.w, wsize_.h, 32);

    if (DxLib_Init() == -1) {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);
    return true;
}

void Application::Run()
{
    Input input;//入力を受け取るためのオブジェクト
    SceneController controller;//シーンを実行するためのオブジェクト

    //最初のシーンとして、タイトルシーンをセットしている
    controller.ChangeScene(std::make_shared<TitleScene>(controller));
    //メインループ
    while (ProcessMessage() != -1 && !requestExit_) {
        
        input.Update();//入力情報の更新

        ClearDrawScreen();//そのうちここのClearDrawScreenは消す

        controller.Update(input);//シーンの更新

        controller.Draw();//シーンの描画

        //ここにゲームの処理を書いていく
        ScreenFlip();
    }

}

void Application::Terminate()
{
    DxLib_End();
}

const Size& Application::GetWindowSize() const
{
    return wsize_;
}

void Application::RequestExit()
{
    requestExit_ = true;
}

int Application::GetBGMVolume() const
{
    return bgmVolume_;
}

void Application::SetBGMVolume(int volume)
{
    bgmVolume_ = volume;
}

int Application::GetSEVolume() const
{
    return seVolume_;
}

void Application::SetSEVolume(int volume)
{
    seVolume_ = volume;
}
