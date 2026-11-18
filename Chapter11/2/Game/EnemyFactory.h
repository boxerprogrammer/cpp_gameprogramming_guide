#pragma once
#include<list>
#include<memory>
#include<vector>
#include<functional>
#include"../Geometry.h"
class Enemy;
enum class EnemyType {
	none,//無効(0)
	zako,//ザコ
	item_bringer,//アイテムを持ってくるやつ
	gunner,//地上から弾を撃つやつ
};
class Player;
class EffectFactory;//エフェクト工場
class BulletFactory;//弾工場
class ItemFactory;//アイテム工場
class GameScene;//それぞれの敵キャラがGameSceneを参照できるようにする(主にステージの内容を取得するため)

/// <summary>
/// 敵生産工場
/// </summary>
class EnemyFactory
{
private:
	std::vector<int> enemyImgs_;//敵の画像ハンドル集(0番目は使用しない)
	//敵オブジェクトを生成して返すラムダ式関数型を定義
	using CreateLambda_t = std::function<std::shared_ptr<Enemy>(int, const Position2&, EnemyType)>;
	//敵を生成するためのラムダ式配列
	using CreateFuncs_t = std::vector<CreateLambda_t>;
	CreateFuncs_t createFuncs_;//敵生成関数(0番目は機能しない)

	std::weak_ptr<GameScene> gameScene_;//ゲームシーンの弱参照
	std::weak_ptr<Player> player_;//プレイヤーの弱参照
	std::shared_ptr<EffectFactory> effectFactory_;//エフェクト工場
	std::shared_ptr<BulletFactory> bulletFactory_;//弾工場
	std::shared_ptr<ItemFactory> itemFactory_;//アイテム工場

	using Enemies_t = std::list<std::shared_ptr<Enemy>>;//敵を一括管理できるようにしたリスト型
	Enemies_t enemies_;//敵一括管理リスト
public:
	/// <summary>
	/// 敵生産工場コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="effectFactory">エフェクト工場</param>
	/// <param name="bulletFactory">弾工場</param>
	/// <param name="gameScene">ゲームシーン</param>
	EnemyFactory(std::weak_ptr<Player> player,
				std::shared_ptr<EffectFactory> effectFactory,
				std::shared_ptr<BulletFactory> bulletFactory,
				std::shared_ptr<ItemFactory> itemFactory,
				std::weak_ptr<GameScene> gameScene);
	void Update();
	void Draw();
	void CreateEnemy(const Position2& pos,EnemyType type);
	Enemies_t& GetEnemies();
};

