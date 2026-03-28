#include "Level.h"
#include <cmath>
#include <fstream>
#include <ctime>
#include <cstdlib>

Level::Level()
{
    m_enemySpeed = 60.0f;
    m_enemiesTagged = 0;
}

Level::~Level()
{
    Shutdown();
}

void Level::SpawnInitialNPCs()
{
    SDL_Point windowSize = Renderer::Instance().GetWindowSize();

    float centerX = windowSize.x * 0.5f;
    float centerY = windowSize.y * 0.5f;

    srand((unsigned int)time(nullptr));

    for (int i = 0; i < 10; i++)
    {
        float angle = ((float)rand() / (float)RAND_MAX) * 6.2831853f;
        float radius = ((float)rand() / (float)RAND_MAX) * 150.0f;

        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius;

        NPC* npc = m_npcPool.GetResource();
        npc->Initialize(x, y);
        npc->SetSpeed(m_enemySpeed);
        m_npcs.push_back(npc);
    }
}

bool Level::Initialize()
{
    SDL_Point windowSize = Renderer::Instance().GetWindowSize();

    if (!m_player.Initialize(windowSize))
    {
        return false;
    }

    m_npcs.clear();
    SpawnInitialNPCs();

    return true;
}

void Level::Update(float _dt)
{
    m_player.Update(_dt);

    std::vector<NPC*> aliveNpcs;

    for (size_t i = 0; i < m_npcs.size(); i++)
    {
        m_npcs[i]->SetSpeed(m_enemySpeed);
        m_npcs[i]->Update(_dt, m_player.GetX(), m_player.GetY());

        if (m_npcs[i]->IsDeadFinished())
        {
            m_enemiesTagged++;
            m_npcPool.ReleaseResource(m_npcs[i]);
        }
        else
        {
            aliveNpcs.push_back(m_npcs[i]);
        }
    }

    m_npcs = aliveNpcs;
}

void Level::Render()
{
    for (size_t i = 0; i < m_npcs.size(); i++)
    {
        m_npcs[i]->Render();
    }

    m_player.Render();
}

void Level::Shutdown()
{
    for (size_t i = 0; i < m_npcs.size(); i++)
    {
        m_npcPool.ReleaseResource(m_npcs[i]);
    }

    m_npcs.clear();
    m_player.Shutdown();
}

float Level::GetPlayerSpeed() const
{
    return m_player.GetSpeed();
}

float Level::GetEnemySpeed() const
{
    return m_enemySpeed;
}

int Level::GetEnemiesTagged() const
{
    return m_enemiesTagged;
}

void Level::IncreaseEnemySpeed()
{
    m_enemySpeed += 10.0f;
    if (m_enemySpeed > 60.0f)
    {
        m_enemySpeed = 60.0f;
    }
}

void Level::DecreaseEnemySpeed()
{
    m_enemySpeed -= 10.0f;
    if (m_enemySpeed < 0.0f)
    {
        m_enemySpeed = 0.0f;
    }
}

void Level::Save()
{
    std::ofstream out("level.bin", std::ios::binary);
    if (!out.is_open())
    {
        return;
    }

    float playerX = m_player.GetX();
    float playerY = m_player.GetY();

    out.write(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    out.write(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    out.write(reinterpret_cast<char*>(&m_enemySpeed), sizeof(m_enemySpeed));
    out.write(reinterpret_cast<char*>(&m_enemiesTagged), sizeof(m_enemiesTagged));

    int npcCount = (int)m_npcs.size();
    out.write(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    for (int i = 0; i < npcCount; i++)
    {
        float x = m_npcs[i]->GetX();
        float y = m_npcs[i]->GetY();

        out.write(reinterpret_cast<char*>(&x), sizeof(x));
        out.write(reinterpret_cast<char*>(&y), sizeof(y));
    }

    out.close();
}

void Level::Load()
{
    std::ifstream in("level.bin", std::ios::binary);
    if (!in.is_open())
    {
        return;
    }

    for (size_t i = 0; i < m_npcs.size(); i++)
    {
        m_npcPool.ReleaseResource(m_npcs[i]);
    }
    m_npcs.clear();

    float playerX = 0.0f;
    float playerY = 0.0f;

    in.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    in.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    in.read(reinterpret_cast<char*>(&m_enemySpeed), sizeof(m_enemySpeed));
    in.read(reinterpret_cast<char*>(&m_enemiesTagged), sizeof(m_enemiesTagged));

    m_player.SetPosition(playerX, playerY);

    int npcCount = 0;
    in.read(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    for (int i = 0; i < npcCount; i++)
    {
        float x = 0.0f;
        float y = 0.0f;

        in.read(reinterpret_cast<char*>(&x), sizeof(x));
        in.read(reinterpret_cast<char*>(&y), sizeof(y));

        NPC* npc = m_npcPool.GetResource();
        npc->Initialize(x, y);
        npc->SetSpeed(m_enemySpeed);
        m_npcs.push_back(npc);
    }

    in.close();
}

bool Level::HasNoEnemiesLeft() const
{
    return m_npcs.empty();
}