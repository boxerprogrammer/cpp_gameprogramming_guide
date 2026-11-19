#pragma once
#include<memory>
#include<list>
#include<vector>
#include<functional>
#include"../Geometry.h"//Positionとか使うため
class Shot;
class GameScene;
//自機ショットタイプ
enum class ShotType {
	normal,//通常弾
	charge,//溜めショット
	missile,//ホーミング弾
	reflect_laser,//反射レーザー
	re_reflect_laser,//反射された反射レーザー
};
/// <summary>
/// 自機のショットを生成・管理する
/// </summary>
class ShotFactory
{
private:
	std::vector<int> shotImgs_;

	using CreateFunc_t = std::function<std::shared_ptr<Shot>(int,const Position2&,const Vector2&)>;
	std::vector<CreateFunc_t> createFuncs_;

	using Shots_t = std::list<std::shared_ptr<Shot>>;
	Shots_t shots_;
	std::weak_ptr<GameScene> gameScene_;
public:
	ShotFactory(std::weak_ptr<GameScene> gs);
	//更新
	void Update();
	//描画
	void Draw();
	/// <summary>
	/// ショットを発生させる
	/// </summary>
	/// <param name="pos">発生座標</param>
	/// <param name="shotType">弾タイプ</param>
	void Fire(const Position2& pos,ShotType shotType,Vector2 vec={});
	/// <summary>
	/// 自分が持ってるショットのリストを返す
	/// </summary>
	/// <returns>ショットのリスト</returns>
	Shots_t& GetShots();
	

};

