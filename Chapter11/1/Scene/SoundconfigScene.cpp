#include<DxLib.h>
#include<algorithm>

#include "KeyconfigScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../Application.h"

#include "SoundconfigScene.h"

constexpr int margin_size = 60;
constexpr int appear_interval = 15;
constexpr int min_volume = 0;
constexpr int max_volume = 100;

constexpr int soundconfig_left = 380;
constexpr int soundconfig_top = 180;
constexpr int soundconfig_text_height = 30;
constexpr int arrow_offset = -30;

constexpr uint32_t shadow_text_col = 0x000000;
constexpr uint32_t white_text_col = 0xffffff;
constexpr uint32_t selected_text_col = 0xffff88;

constexpr uint32_t back_color = 0xaa8844;
constexpr uint32_t frame_color = 0xaaaaff;


void SoundconfigScene::AppearUpdate(Input&)
{
	if (++frame_ >= appear_interval) {
		update_ = &SoundconfigScene::NormalUpdate;
		draw_ = &SoundconfigScene::NormalDraw;
		return;
	}
}

void SoundconfigScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("up")) {
		selectedIndex_ = static_cast<int>((selectedIndex_ + menuItems_.size() - 1) % menuItems_.size());
	}
	if (input.IsTriggered("down")) {
		selectedIndex_ = (selectedIndex_ + 1) % menuItems_.size();
	}
	menuFunction_[menuItems_[selectedIndex_]](input);
}

void SoundconfigScene::DisappearUpdate(Input&)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}
void SoundconfigScene::DrawStringWithShadow(int x, int y, uint32_t textCol, const std::wstring& text)
{
	DrawFormatString(x + 2, y + 2, shadow_text_col, L"%s", text.c_str());//影
	DrawFormatString(x, y, textCol, L"%s", text.c_str());//本体
}

void SoundconfigScene::ChangeVolume(SoundType type, int changeValue)
{
	auto& volume = tempVolumes_[static_cast<int>(type)];
	volume = std::clamp(volume + changeValue, min_volume, max_volume);
}

int SoundconfigScene::GetChangeValue(const Input& input)
{
	if (input.IsPressed("left")) {
		return -1;
	}
	if (input.IsPressed("right")) {
		return 1;
	}
	return 0;
}

void SoundconfigScene::NormalDraw()
{
	TransitionEffectDraw();
	DrawString(margin_size + 50, margin_size + 50, L"SoundconfigScene Scene", white_text_col);
	
	int y = margin_size + soundconfig_top;

	int idx = 0;
	for (const auto& item:menuItems_) {
		int x = margin_size + soundconfig_left;
		uint32_t textCol = white_text_col;
		if (item == menuItems_[selectedIndex_]) {
			textCol = selected_text_col;
			DrawStringWithShadow(x+arrow_offset, y, textCol, L"☞");
			x += 5;
		}
		std::array<wchar_t,256> ctext;//一時配列(ちょっと大きめに確保)
		if (idx < tempVolumes_.size()) {
			wsprintf(ctext.data(), L"%s : %d", item.c_str(), tempVolumes_[idx]);
		}
		else {
			wsprintf(ctext.data(), L"%s", item.c_str());
		}
		DrawStringWithShadow(x, y, textCol, ctext.data());
		++idx;
		y += soundconfig_text_height;
	}

}

void SoundconfigScene::TransitionEffectDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto center_x = wsize.w / 2;//横開きなので、画面中央を計算
	const auto frame_max_width = wsize.w / 2 - margin_size;//最大広がった時の広さ÷2
	//現在の経過時間を0～1に変換する
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);
	//rate=0.0のときcurrentWidth=0、rate=1.0のときcurrentWidth=frame_max_width
	int currentWidth = static_cast<int>(frame_max_width * rate);
	//黄色っぽい面(一番下にある面で、テキストを乗せるためのもの)
	DrawBox(center_x - currentWidth,
		margin_size,
		center_x + currentWidth,
		wsize.h - margin_size,
		back_color, true);
	//枠
	DrawBox(center_x - currentWidth,
		margin_size,
		center_x + currentWidth,
		wsize.h - margin_size,
		frame_color, false, 3);
}

SoundconfigScene::SoundconfigScene(SceneController& ctrl) : 
Scene(ctrl),
frame_(0)//frame_に初期値(0)を入れておく
{
	update_ = &SoundconfigScene::AppearUpdate;//表示演出
	draw_ = &SoundconfigScene::TransitionEffectDraw;//演出中描画
	
	const auto& app = Application::GetInstance();
	tempVolumes_[static_cast<int>(SoundType::bgm)] = app.GetBGMVolume();
	tempVolumes_[static_cast<int>(SoundType::se)]= app.GetSEVolume();

	menuItems_ = { L"BGMボリューム",L"SEボリューム", 
					L"変更を反映して閉じる",L"キャンセル",L"リセットして閉じる"};
	menuFunction_[L"BGMボリューム"] = [this](Input& input) {
		ChangeVolume(SoundType::bgm, GetChangeValue(input));
	};
	menuFunction_[L"SEボリューム"] = [this](Input& input) {
		ChangeVolume(SoundType::se, GetChangeValue(input));
	};
	menuFunction_[L"変更を反映して閉じる"] = [this](Input& input) {
		if (input.IsTriggered("ok")) {
			Application::GetInstance().SetBGMVolume(tempVolumes_[static_cast<int>(SoundType::bgm)]);
			Application::GetInstance().SetSEVolume(tempVolumes_[static_cast<int>(SoundType::se)]);
			CloseWindow();
		}
	};
	menuFunction_[L"リセットして閉じる"] = [this](Input& input) {
		if (input.IsTriggered("ok")) {
			Application::GetInstance().SetBGMVolume(max_volume);
			Application::GetInstance().SetSEVolume(max_volume);
			CloseWindow();
		}
	};
	menuFunction_[L"キャンセル"] = [this](Input& input) {
		if (input.IsTriggered("ok")) {
			CloseWindow();
		}
	};

}

void SoundconfigScene::CloseWindow()
{
	update_ = &SoundconfigScene::DisappearUpdate;
	draw_ = &SoundconfigScene::TransitionEffectDraw;
	frame_ = appear_interval;
}

void SoundconfigScene::Update(Input& input)
{
	(this->*update_)(input);
}

void SoundconfigScene::Draw()
{
	(this->*draw_)();
}
