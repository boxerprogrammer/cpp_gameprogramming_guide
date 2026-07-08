#include "EnemyFactory.h"
#include"Zako.h"
#include"PataPata.h"
#include"Strider.h"
#include"ZakoSpawner.h"
#include"Boss.h"
#include<DxLib.h>
#include"../Application.h"
#include"../ResourceManager.h"
#include<cassert>

constexpr int cell_size = 32;//セルサイズ(マップの１セルの大きさ)
constexpr int half_cell_size = 32/2;//セルサイズの半分

EnemyFactory::EnemyFactory(std::shared_ptr<Player> player,std::shared_ptr<BulletFactory> bf,std::shared_ptr<EffectFactory> ef):
        player_(player),
        bulletFactory_(bf),
        effectFactory_(ef)
{
    //コンストラクタ内で、必要な敵の画像をすべてロードしておく
    //敵の表示に必要な画像のハンドルをあらかじめロードしておく
    int handle = -1;
    enemyImageTable_[EnemyType::zako] = mylib::LoadTexture(L"img/game/zako.png");//1番(雑魚)
    enemyImageTable_[EnemyType::patapata] = mylib::LoadTexture(L"img/game/patapata.png");//2番(パタパタ)
    enemyImageTable_[EnemyType::strider] = mylib::LoadTexture(L"img/game/strider.png");//3番(ストライダー)
    enemyImageTable_[EnemyType::boss1] = mylib::LoadTexture(L"img/game/boss.png");//15番(1面ボス)
}

std::shared_ptr<Enemy> 
EnemyFactory::Create(int idxX,int idxY, EnemyType enemyType)
{
    const auto& wsize = Application::GetInstance().GetWindowSize();
    if (enemyType == EnemyType::none) {
        return nullptr;
    }
    //配置されたセルの場所の中心に表示しようとします
    Position2 pos = { idxX * cell_size+half_cell_size ,//
                        wsize.h - cell_size//最下段のセルの上端
                        - idxY * cell_size //そこからセル番号*セルサイズを引く
                        + half_cell_size};//セルの半分を足してあげる
    switch (enemyType) {
        case EnemyType::zako://雑魚１
            enemies_.push_back(std::make_shared<Zako>(
                                enemyImageTable_[EnemyType::zako],//雑魚のハンドル
                                player_,//プレイヤーへのポインタ
                                bulletFactory_,//弾生産工場のポインタ
                                effectFactory_,
                                pos));//管理できるように内部のlistに登録
            return enemies_.back();//今追加したばかりの雑魚を返す
        case EnemyType::patapata://パタパタ
            enemies_.push_back(std::make_shared<PataPata>(
                enemyImageTable_[EnemyType::patapata],//パタパタのハンドル
                player_,//プレイヤーへのポインタ
                bulletFactory_,//弾生産工場のポインタ
                effectFactory_,
                pos));//管理できるように内部のlistに登録
            return enemies_.back();
        case EnemyType::strider://ストライダー
            enemies_.push_back(std::make_shared<Strider>(
                enemyImageTable_[EnemyType::strider],//ストライダーのハンドル
                player_,//プレイヤーへのポインタ
                bulletFactory_,//弾生産工場のポインタ
                effectFactory_,
                pos));//管理できるように内部のlistに登録
            return enemies_.back();
        case EnemyType::zako_spawner:
            enemies_.push_back(std::make_shared<ZakoSpawner>(
                *this,
                pos));//管理できるように内部のlistに登録
            return enemies_.back();
        case EnemyType::boss1:
            enemies_.push_back(std::make_shared<Boss>(
                enemyImageTable_[EnemyType::boss1],//ボスのハンドル
                player_,//プレイヤーへのポインタ
                bulletFactory_,//弾生産工場のポインタ
                effectFactory_,
                pos));//管理できるように内部のlistに登録
            return enemies_.back();
        default:
            return nullptr;
    }
    return nullptr;
}

std::list<std::shared_ptr<Enemy>>& EnemyFactory::GetEnemies()
{
    return enemies_;
}

void 
EnemyFactory::Update() {
    for (auto& enemy : enemies_) {
        enemy->Update();
    }
}
void EnemyFactory::Draw() {
    for (auto& enemy : enemies_) {
        enemy->Draw();
    }
}

EnemyFactory::~EnemyFactory()
{
    for (const auto& pair : enemyImageTable_) {
        DeleteGraph(pair.second);
    }
}
