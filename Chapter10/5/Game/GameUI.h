#pragma once

class Score;
class PlayerStatus;
class GameData;

/// <summary>
/// ゲームプレイ中の HUDを描画するクラス。
/// </summary>
class GameUI
{
public:
    // フィールドとパネルの境界 X 座標
    static constexpr int field_right = 448;
    static constexpr int ui_panel_left  = field_right;
    static constexpr int ui_panel_width = 640-ui_panel_left; // 

    /// <summary>
    /// コンストラクタ。参照を束縛し、フォントをロードする。
    /// GameScene のコンストラクタで生成すること。
    /// </summary>
    GameUI(const Score& score,
           const PlayerStatus& playerStatus,
           const GameData& gameData);

    ~GameUI();

    /// <summary>
    /// 右パネルを描画する。GameScene::Draw() の末尾で呼ぶこと。
    /// （ゲームオブジェクトより手前に表示するため）
    /// </summary>
    void Draw() const;

private:
    // 描画に使うデータへの const 参照（所有しない）
    const Score&         score_;
    const PlayerStatus&  playerStatus_;
    const GameData&      gameData_;

    // フォントハンドル（GameUI が所有・破棄する）
    int fontH_ = -1;

    // 内部描画ヘルパー
    void DrawPanel()      const; // パネル背景
    void DrawScores()     const; // ハイスコア・現在スコア
    void DrawLives()      const; // 残機アイコン
    void DrawBombs()      const; // 残ボムアイコン
    void DrawGraze()      const; // グレイズカウント

    /// <summary>影付き文字列を描画する（東方風）</summary>
    void DrawShadowedString(int x, int y,
                             const wchar_t* str,
                             unsigned int color = 0xffffff) const;
};