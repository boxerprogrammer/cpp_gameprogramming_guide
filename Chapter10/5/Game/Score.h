#pragma once
#include<string>
/// <summary>
/// スコア関連の処理をまとめたクラス
/// </summary>
/// <remarks>
/// 「今プレイ中のスコア」だけを数えるシンプルな責務
/// </remarks>
class Score {
	uint64_t currentScore_ = 0;//現在のスコア
    uint32_t graze_ = 0;//グレイズスコア
public:
    void Add(uint64_t score);
    void AddGraze(uint32_t graze = 1);
    uint64_t GetCurrent() const;
    uint32_t GetGraze() const;
};