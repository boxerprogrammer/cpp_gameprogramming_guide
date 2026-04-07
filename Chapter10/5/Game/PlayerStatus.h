#pragma once

/// <summary>
/// プレイヤーの残機・残ボム・レベルを管理するクラス。
/// </summary>
class PlayerStatus
{
public:
    // ゲーム開始時の初期値
    static constexpr int initial_lives = 2;   // 初期残機（自機ストック）
    static constexpr int initial_bombs = 3;   // 初期残ボム数
    static constexpr int max_lives     = 8;   // 残機の上限（アイテム取得時に使用）
    static constexpr int max_bombs     = 8;   // 残ボムの上限
	static constexpr int max_player_level = 100;   // レベルの上限（アイテム取得時に使用）

    PlayerStatus() = default;

    // -------------------------------------------------------------------------
    // GameScene のゲームロジックから呼ぶ
    // -------------------------------------------------------------------------
    
	void AddPlayerLevel();
	void ResetPlayerLevel();

    /// <summary>
    /// 1機失う。残機が 0 の場合はゲームオーバー判定に使う。
    /// </summary>
    void LoseLife();

    /// <summary>
    /// ボムを1つ使用する。残ボムが 0 のときは何もしない。
    /// </summary>
    /// <returns>ボムを使用できた場合 true</returns>
    bool UseBomb();

    /// <summary>残機を1増やす（エクステンド時）</summary>
    void ExtendLife();

    /// <summary>残ボムを1増やす（ボムアイテム取得時）</summary>
    void AddBomb();

    // -------------------------------------------------------------------------
    // GameUI などから読み取りに呼ぶ
    // -------------------------------------------------------------------------

    int  GetLives() const;
    int  GetBombs() const;

    /// <summary>残機が0かどうかを返す（ゲームオーバー判定）</summary>
    bool IsGameOver() const;

private:
    int lives_ = initial_lives;
    int bombs_ = initial_bombs;
	int playerLevel_ = 1; // プレイヤーのレベル
};