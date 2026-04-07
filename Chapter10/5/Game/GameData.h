#pragma once
#include <string>
#include <vector>

/// <summary>
/// 1プレイの結果を表す1件のランキングエントリ
/// </summary>
struct RankEntry
{
    long long score = 0;
    std::string name;  // プレイヤー名（名前入力画面で設定）
};

/// <summary>
/// シーンをまたいで保持が必要なデータを一元管理するクラス。
/// </summary>
class GameData
{
public:
    // ランキングに保存する最大件数
    static constexpr int max_ranking_entries = 10;

    GameData() = default;

    // -------------------------------------------------------------------------
    // GameScene から呼ぶ
    // -------------------------------------------------------------------------

    /// <summary>
    /// 直前プレイのスコアを記録する。
    /// GameScene の終了直前（フェードアウト完了時）に呼ぶこと。
    /// ハイスコアの更新も内部で行う。
    /// </summary>
    /// <param name="score">Score::GetCurrent() で取得した値を渡す</param>
    void SetLastScore(long long score);

    // -------------------------------------------------------------------------
    // 各シーンから参照する
    // -------------------------------------------------------------------------

    /// <summary>直前プレイのスコアを返す（ゲームオーバー画面・ランキング登録で使用）</summary>
    long long GetLastScore() const;

    /// <summary>全プレイを通じた最高スコアを返す</summary>
    long long GetHighScore() const;

    /// <summary>ランキング一覧への読み取り専用参照を返す</summary>
    const std::vector<RankEntry>& GetRanking() const;

    // -------------------------------------------------------------------------
    // RankingScene から呼ぶ
    // -------------------------------------------------------------------------

    /// <summary>
    /// 直前プレイのスコアをランキングに登録する。
    /// 登録後は降順ソートし、上位 ｎ 件のみ保持する。
    /// </summary>
    /// <param name="name">名前入力画面で入力されたプレイヤー名</param>
    void RegisterScore(const std::string& name);

    /// <summary>
    /// 直前プレイのスコアがランキング圏内かどうかを判定する。
    /// 名前入力画面を表示するか否かの判断に使う。
    /// </summary>
    bool IsInRanking() const;

    // -------------------------------------------------------------------------
    // ファイル I/O
    // -------------------------------------------------------------------------

    /// <summary>
    /// ランキングデータをファイルから読み込む。
    /// Application::Run() の開始直後に呼ぶこと。
    /// </summary>
    /// <param name="filePath">ランキングファイルのパス（例: "ranking.dat"）</param>
    void LoadRanking(const std::string& filePath);

    /// <summary>
    /// ランキングデータをファイルに書き込む。
    /// RegisterScore() の後、または Application::Terminate() の前に呼ぶこと。
    /// </summary>
    /// <param name="filePath">ランキングファイルのパス（例: "ranking.dat"）</param>
    void SaveRanking(const std::string& filePath) const;

private:
    long long score_ = 0;           // 直前プレイのスコア
    long long highScore_ = 0;           // 全時代最高スコア
    std::vector<RankEntry> ranking_;    // ランキング(vectorなのはあとでセーブする時のため)
};
