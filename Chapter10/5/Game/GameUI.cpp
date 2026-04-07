#include "GameUI.h"
#include "Score.h"
#include "PlayerStatus.h"
#include "GameData.h"
#include <DxLib.h>

// パネル内のレイアウト定数（x は kPanelLeft からの相対ではなく絶対座標）
namespace {
    constexpr int x_start_text       = GameUI::ui_panel_left + 16; // 文字列の描画開始 X
    constexpr int x_start_icon       = GameUI::ui_panel_left + 16; // アイコン描画開始 X
    constexpr int icon_size    = 20;  // アイコン1個あたりのサイズ (px)
    constexpr int icon_space = 22;  // アイコン間隔 (px)

    // 各項目の Y 座標
    constexpr int hiscore_label_y  =  20;
    constexpr int hiscore_value_y  =  40;
    constexpr int score_label_y    =  80;
    constexpr int score_value_y    = 100;
    constexpr int player_label_y   = 160;
    constexpr int player_icon_y    = 182;
    constexpr int bomb_label_y     = 230;
    constexpr int bomb_icon_y      = 252;
    constexpr int graze_label_y    = 310;
    constexpr int graze_value_y    = 330;

    // パネル背景色（半透明の黒）
    constexpr unsigned int kPanelColor  = 0x000000;
    constexpr int          kPanelAlpha  = 180;

    // テキスト色
    constexpr unsigned int kColorLabel  = 0xaaaaaa; // ラベル（薄いグレー）
    constexpr unsigned int kColorValue  = 0xffffff; // 値（白）
    constexpr unsigned int kColorHi     = 0xffdd00; // ハイスコア（金色）

    // アイコン色
    constexpr unsigned int kColorLife   = 0xff6666; // 残機（赤）
    constexpr unsigned int kColorBomb   = 0x66aaff; // 残ボム（青）
    constexpr unsigned int kColorEmpty  = 0x444444; // 空スロット（暗いグレー）
}

// ----------------------------------------------------------------------------
// コンストラクタ・デストラクタ
// ----------------------------------------------------------------------------

GameUI::GameUI(const Score& score,
               const PlayerStatus& playerStatus,
               const GameData& gameData)
    : score_(score)
    , playerStatus_(playerStatus)
    , gameData_(gameData)
{

}

GameUI::~GameUI()
{

}

// ----------------------------------------------------------------------------
// Draw() - 外部から呼ぶ唯一の公開メンバ
// ----------------------------------------------------------------------------

void GameUI::Draw() const
{
    DrawPanel();
    DrawScores();
    DrawLives();
    DrawBombs();
    DrawGraze();
}

// ----------------------------------------------------------------------------
// 内部ヘルパー
// ----------------------------------------------------------------------------

void GameUI::DrawPanel() const
{
    // 半透明の黒い背景をパネル領域に描画する
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, kPanelAlpha);
    DrawBox(ui_panel_left, 0, 640, 480, kPanelColor, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // パネル左端に縦線を引く（境界を明示する）
    DrawLine(ui_panel_left, 0, ui_panel_left, 480, 0x555555);
}

void GameUI::DrawScores() const
{
    // ---- HI SCORE ----
    DrawShadowedString(x_start_text, hiscore_label_y, L"HI SCORE", kColorLabel);
    DrawFormatString(
        x_start_text, hiscore_value_y,
        kColorHi, 
        L"%9lld", gameData_.GetHighScore());

    // ---- SCORE ----
    DrawShadowedString(x_start_text, score_label_y, L"SCORE", kColorLabel);
    DrawFormatString(
        x_start_text, score_value_y,
        kColorValue,
        L"%9lld", score_.GetCurrent());
}

void GameUI::DrawLives() const
{
    DrawShadowedString(x_start_text, player_label_y, L"PLAYER", kColorLabel);

    const int lives = playerStatus_.GetLives();

    for (int i = 0; i < PlayerStatus::max_lives; ++i) {
        // 最大表示数を超えたら打ち切る（アイコンがはみ出さないよう）
        if (x_start_icon + i * icon_space + icon_size > 640) break;

        const unsigned int color = (i < lives) ? kColorLife : kColorEmpty;
        const int x = x_start_icon + i * icon_space;
        const int y = player_icon_y;

        // ♥ 型のアイコンを三角形2枚で近似する（画像なしで描画する）
        // 上部の2つの半円を省略し、ひし形で代用（シンプル実装）
        DrawTriangle(
            x + icon_size / 2, y,
            x,                 y + icon_size / 3,
            x + icon_size,     y + icon_size / 3,
            color, TRUE);
        DrawTriangle(
            x,                 y + icon_size / 3,
            x + icon_size,     y + icon_size / 3,
            x + icon_size / 2, y + icon_size,
            color, TRUE);
    }
}

void GameUI::DrawBombs() const
{
    DrawShadowedString(x_start_text, bomb_label_y, L"BOMB", kColorLabel);

    const int bombs = playerStatus_.GetBombs();

    for (int i = 0; i < PlayerStatus::max_bombs; ++i) {
        if (x_start_icon + i * icon_space + icon_size > 640) break;

        const unsigned int color = (i < bombs) ? kColorBomb : kColorEmpty;
        const int x = x_start_icon + i * icon_space;
        const int y = bomb_icon_y;
        const int cx = x + icon_size / 2;
        const int cy = y + icon_size / 2;
        const int r  = icon_size / 2;

        // 💎 型のアイコンを菱形で代用（シンプル実装）
        DrawQuadrangleAA(
            static_cast<float>(cx),     static_cast<float>(y),
            static_cast<float>(x + r * 2), static_cast<float>(cy),
            static_cast<float>(cx),     static_cast<float>(y + icon_size),
            static_cast<float>(x),      static_cast<float>(cy),
            color, TRUE);
    }
}

void GameUI::DrawGraze() const
{
    DrawShadowedString(x_start_text, graze_label_y, L"GRAZE", kColorLabel);
    DrawFormatStringToHandle(
        x_start_text, graze_value_y,
        kColorValue, fontH_,
        L"%d", score_.GetGraze());
}

void GameUI::DrawShadowedString(int x, int y,
                                 const wchar_t* str,
                                 unsigned int color) const
{
    constexpr int kShadowOffset = 2;

    if (fontH_ >= 0) {
        DrawStringToHandle(x + kShadowOffset, y + kShadowOffset,
                           str, 0x333333, fontH_);
        DrawStringToHandle(x, y, str, color, fontH_);
    }
    else {
        // フォントが未ロードでも最低限表示できるようフォールバック
        DrawString(x, y, str, color);
    }
}