#include "GameData.h"
#include<algorithm>
#include<fstream>


void GameData::SetLastScore(long long score)
{
	score_ = score;
}

long long GameData::GetLastScore() const
{
	return score_;
}

long long GameData::GetHighScore() const
{
	return highScore_;
}

const std::vector<RankEntry>& GameData::GetRanking() const
{
	return ranking_;
}

void GameData::RegisterScore(const std::string& name)
{
	ranking_.push_back({ score_, name });
	std::sort(ranking_.begin(), ranking_.end(),
		[](const RankEntry& a, const RankEntry& b) {
			return a.score > b.score; // 降順ソート
		});
	if(ranking_.size() > max_ranking_entries) {
		ranking_.pop_back();
	}
}

bool GameData::IsInRanking() const
{
	//　ランキングが満員でない場合は常にランキング入り
	if(static_cast<int>(ranking_.size()) < max_ranking_entries) {
		return true;
	}
	// ランキングが満員の場合は、現在のスコアが最下位より高いかどうかで判断
	return score_ > ranking_.back().score;
}

void GameData::LoadRanking(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        // ファイルが存在しない（初回起動など）場合は空のまま続行
        return;
    }

    // ランキング件数を読む
    int count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    count = std::min(count, max_ranking_entries);  // 念のため上限を守る

    ranking_.clear();
    ranking_.reserve(count);

    for (int i = 0; i < count; ++i) {
        RankEntry entry;

        // スコア（8バイト固定）
        file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));

        // 名前（先頭4バイトが文字数、続いて文字列データ）
        int nameLen = 0;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        entry.name.resize(nameLen);
        file.read(entry.name.data(), nameLen);

        ranking_.push_back(entry);
    }

    // ハイスコアをランキングの1位から復元する
    if (!ranking_.empty()) {
        highScore_ = ranking_.front().score;
    }
}

void GameData::SaveRanking(const std::string& filePath) const
{
    std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
    if (!file) {
        return;  // 書き込み失敗時はサイレントに無視（本番ではエラーログ推奨）
    }

    // ランキング件数
    const int count = static_cast<int>(ranking_.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& entry : ranking_) {
        // スコア
        file.write(reinterpret_cast<const char*>(&entry.score), sizeof(entry.score));

        // 名前
        const int nameLen = static_cast<int>(entry.name.size());
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        file.write(entry.name.data(), nameLen);
    }
}
