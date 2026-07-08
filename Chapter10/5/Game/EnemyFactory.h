#pragma once
#include<memory>
#include<list>
#include<vector>
#include<map>


/// <summary>
/// 敵タイプ
/// </summary>
enum class EnemyType {
	none,//敵なし
	zako,//雑魚
	patapata,//パタパタ
	strider,//ストライダー
	zako_spawner,//雑魚発生装置
	boss1=15,//1面ボス
};
//プロトタイプ宣言
class Enemy;
class Player;
class BulletFactory;
class EffectFactory;
/// <summary>
/// 敵生産工場
/// </summary>
class EnemyFactory
{
private:
	//あらかじめ敵の表示に必要な画像データをロードして
	//保持しておく
	//敵全員のハンドルを持っておく
	//vectorでなくmapで持っておくと、敵の種類が増えたときに柔軟に対応できる
	std::map<EnemyType,int> enemyImageTable_;

	std::shared_ptr<BulletFactory> bulletFactory_;//Enemyに渡す弾生産工場のポインタ
	std::shared_ptr<EffectFactory> effectFactory_;//Enemyに渡すエフェクト生産工場
	std::list<std::shared_ptr<Enemy>> enemies_;//管理すべき敵集合
	std::shared_ptr<Player> player_;
public:
	EnemyFactory(std::shared_ptr<Player> player,std::shared_ptr<BulletFactory> bf,std::shared_ptr<EffectFactory> ef);
	/// <summary>
	/// 敵の生成を行う
	/// </summary>
	/// <param name="idxX">マップセルのどこに配置するのかX</param>
	/// <param name="idxY">マップセルのどこに配置するのか</param>
	/// <param name="enemyType">敵タイプ</param>
	/// <param name="player">プレイヤーのポインタ(自機狙い用)</param>
	/// <returns>敵ポインタ</returns>
	std::shared_ptr<Enemy> 
		Create(int idxX, int idxY, EnemyType enemyType);
	/// <summary>
	/// 敵の集合体を返す
	/// </summary>
	/// <returns>敵のリスト</returns>
	std::list<std::shared_ptr<Enemy>>& GetEnemies();
	/// <summary>
	/// 敵全員Update
	/// </summary>
	void Update();
	/// <summary>
	/// 敵全員Draw
	/// </summary>
	void Draw();
	~EnemyFactory();
};

