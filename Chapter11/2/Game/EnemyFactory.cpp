#include "EnemyFactory.h"
#include"Enemy.h"
#include"Zako.h"
#include"ItemBringer.h"
#include"Gunner.h"
#include"../Application.h"
#include"../FileLoader.h"
#include<algorithm>

EnemyFactory::EnemyFactory(std::weak_ptr<Player> player,
                            std::shared_ptr<EffectFactory> effectFactory,
                            std::shared_ptr<BulletFactory> bulletFactory,
                            std::shared_ptr<ItemFactory> itemFactory,
                            std::weak_ptr<GameScene> gameScene):
    player_(player),
    effectFactory_(effectFactory),
    bulletFactory_(bulletFactory),
    itemFactory_(itemFactory),
    gameScene_(gameScene)
{
    auto loader = Application::GetInstance().GetLoader();
    enemyImgs_.push_back(-1);
    enemyImgs_.push_back(loader->LoadGraph(L"img/Actor/zako.png",AssetType::temporary));
    enemyImgs_.push_back(loader->LoadGraph(L"img/Actor/bringer.png", AssetType::temporary));
    enemyImgs_.push_back(loader->LoadGraph(L"img/Actor/gunner.png", AssetType::temporary));

    createFuncs_.push_back([](int, const Position2&, EnemyType) {return std::shared_ptr<Enemy>();});
    createFuncs_.push_back([this](int handle,const Position2& pos, EnemyType type) {
        return std::make_shared<Zako>(handle, pos, player_, effectFactory_, bulletFactory_);
        });
    createFuncs_.push_back([this](int handle, const Position2& pos, EnemyType type) {
        return std::make_shared<ItemBringer>
            (handle, pos, player_, effectFactory_, bulletFactory_,gameScene_,itemFactory_);
        });
    createFuncs_.push_back([this](int handle, const Position2& pos, EnemyType type) {
        return std::make_shared<Gunner>(handle, pos, player_, effectFactory_, bulletFactory_,gameScene_);
        });
}


void EnemyFactory::Update()
{
    for (auto& enemy : enemies_) {
        enemy->Update();
    }
    std::erase_if(enemies_,
        [](const auto& enemy) {
            return enemy->IsDead();
        });
}

void EnemyFactory::Draw()
{
    for (auto& enemy : enemies_) {
        enemy->Draw();
    }
}

void EnemyFactory::CreateEnemy(const Position2& pos, EnemyType type)
{
    int enemyTypeIdx = static_cast<int>(type);
    auto handle = enemyImgs_[enemyTypeIdx];
    enemies_.push_back(createFuncs_[enemyTypeIdx](handle,pos,type));
    auto& enemy = enemies_.back();
    enemy->Init();
}

EnemyFactory::Enemies_t& EnemyFactory::GetEnemies()
{
    return enemies_;
}
