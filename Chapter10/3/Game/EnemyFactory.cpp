#include "EnemyFactory.h"
#include"Zako.h"
#include"PataPata.h"
#include"Strider.h"
#include<DxLib.h>
#include"../Application.h"
#include<cassert>

constexpr int cell_size = 32;//セルサイズ(マップの１セルの大きさ)
constexpr int half_cell_size = 32/2;//セルサイズの半分

EnemyFactory::EnemyFactory(std::shared_ptr<BulletFactory> bf,std::shared_ptr<EffectFactory> ef):
        bulletFactory_(bf),
        effectFactory_(ef)
{
    //コンストラクタ内で、必要な敵の画像をすべてロードしておく
    //敵の表示に必要な画像のハンドルをあらかじめロードしておく
    handles_.push_back(-1);//何もない(使用しない)
    int handle = -1;
    handle = LoadGraph(L"resource/img/game/zako.png");//1番(雑魚)
    assert(handle >= 0);//←いま追加したばかりのハンドルのチェック
    handles_.push_back(handle);//1番
    handle = LoadGraph(L"resource/img/game/patapata.png");//2番(パタパタ)
    assert(handle >= 0);//←いま追加したばかりのハンドルのチェック
    handles_.push_back(handle);//2番
    handle = LoadGraph(L"resource/img/game/strider.png");//3番(ストライダー)
    assert(handle >= 0);//←いま追加したばかりのハンドルのチェック
    handles_.push_back(handle);//3番
}

std::shared_ptr<Enemy> EnemyFactory::Create(
                            int idxX,
                            int idxY, 
                            EnemyType enemyType,
                            std::shared_ptr<Player> player)
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
                                handles_[(int)EnemyType::zako],//雑魚のハンドル
                                player,//プレイヤーへのポインタ
                                bulletFactory_,//弾生産工場のポインタ
                                effectFactory_,
                                pos));//管理できるように内部のlistに登録
            return enemies_.back();//今追加したばかりの雑魚を返す
        case EnemyType::patapata://パタパタ
            enemies_.push_back(std::make_shared<PataPata>(
                handles_[(int)EnemyType::patapata],//パタパタのハンドル
                player,//プレイヤーへのポインタ
                bulletFactory_,//弾生産工場のポインタ
                effectFactory_,
                pos));//管理できるように内部のlistに登録
            return enemies_.back();
        case EnemyType::strider://ストライダー
            enemies_.push_back(std::make_shared<Strider>(
                handles_[(int)EnemyType::strider],//パタパタのハンドル
                player,//プレイヤーへのポインタ
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
    for (int handle : handles_) {
        DeleteGraph(handle);
    }
}
