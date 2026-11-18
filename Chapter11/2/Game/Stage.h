#pragma once
#include<vector>
#include<string>
#include"../Geometry.h"
#include<memory>

enum class LayerType {
	terrain,//地形データ
	enemy,//敵データ
};
class Actor;
/// <summary>
/// ステージデータクラス
/// </summary>
class Stage
{
private:
	int handle_=-1;//背景チップ用ハンドル
	Size mapSize_;//マップサイズ(ステージの横x縦)
	//レイヤー数xデータ
	//データそのものは１次元でよい
	using StageData_t = std::vector<std::vector<uint8_t>>;
	StageData_t data_;
	int scroll_ = 0;//現在のスクロール量

	// データを転置(縦横いれかえる)
	// rawData : 生データ
	// data : 縦横入れ替えられた本データ
	void TransposeData(const StageData_t& rawData, StageData_t& data);
public:
	/// <summary>
	/// ステージデータのロード
	/// </summary>
	/// <param name="stateIndex">ステージのインデックス</param>
	void Load(int stateIndex);
	/// <summary>
	/// ステージデータを丸ごと返す
	/// </summary>
	/// <returns>ステージデータ丸ごと</returns>
	const StageData_t& Data()const;
	/// <summary>
	/// ある列のデータ１列を取得する
	/// </summary>
	/// <param name="layer">レイヤー№</param>
	/// <param name="index">何列目か</param>
	/// <returns>その列のデータのコピー</returns>
	std::vector<uint8_t> GetColumn(LayerType layer,int index)const;
	/// <summary>
	/// 現在どれくらいスクロールが進んでいるか
	/// </summary>
	/// <returns>スクロール数</returns>
	int GetCurrentScroll()const;
	/// <summary>
	/// 更新(スクロール数等)
	/// </summary>
	void Update();
	/// <summary>
	/// ステージマップチップの描画
	/// </summary>
	void Draw();

	///チップサイズ(画面上のチップサイズ)を返す
	int GetChipWidth()const;

	/// <summary>
	/// 背景とアクターが衝突しているか
	/// </summary>
	/// <param name="actor">判定したい相手アクター</param>
	/// <param name="offset">ずらして判定したいオフセットベクトル</param>
	/// <returns>当たってるかどうか</returns>
	bool IsHitTerrain(std::shared_ptr<Actor> actor, const Vector2& offset = {})const;

	/// <summary>
	/// 背景とアクターを当たり判定し、当たっていればブロックする
	/// (めり込ませない(位置の補正をする))
	/// </summary>
	/// <param name="actor">アクター</param>
	/// <param name="offset">オフセット</param>
	/// <returns>当たってるかどうか</returns>
	bool BlockTerrain(std::shared_ptr<Actor> actor, const Vector2& offset = {})const;

};

