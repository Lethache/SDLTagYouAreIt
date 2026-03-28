#ifndef LEVEL_H
#define LEVEL_H

#include "StandartIncludes.h"
#include "Player.h"
#include "NPC.h"
#include "Renderer.h"
#include "ObjectPool.h"

class Level
{
public:
    Level();
    ~Level();

    bool Initialize();
    void Update(float _dt);
    void Render();
    void Shutdown();

    float GetPlayerSpeed() const;
    float GetEnemySpeed() const;
    int GetEnemiesTagged() const;

    void IncreaseEnemySpeed();
    void DecreaseEnemySpeed();

    void Save();
    void Load();

    bool HasNoEnemiesLeft() const;

private:
    void SpawnInitialNPCs();

private:
    Player m_player;
    std::vector<NPC*> m_npcs;
    ObjectPool<NPC> m_npcPool;

    float m_enemySpeed;
    int m_enemiesTagged;
};

#endif