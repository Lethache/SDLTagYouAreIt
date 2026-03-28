#include "GameController.h"
#include "Renderer.h"
#include "Timing.h"
#include "Level.h"
#include "AssetController.h"
#include "TTFont.h"
#include "AudioController.h"
#include "SoundEffect.h"

GameController::GameController()
{
    m_renderer = nullptr;
    m_level = nullptr;
    m_font = nullptr;
    m_audio = nullptr;
    m_whoosh = nullptr;
    m_quit = false;
}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
    AssetController::Instance().Initialize(100000000);

    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    Timing::Instance().SetFPS(80);

    m_font = new TTFont();
    m_font->Initialize(20);

    m_audio = &AudioController::Instance();
    m_whoosh = m_audio->LoadEffect("./Assets/Audio/Effects/Whoosh.wav");

    m_level = new Level();
    m_level->Initialize();
}

void GameController::ShutDown()
{
    if (m_font != nullptr)
    {
        delete m_font;
        m_font = nullptr;
    }

    if (m_level != nullptr)
    {
        m_level->Shutdown();
        delete m_level;
        m_level = nullptr;
    }
}

void GameController::PlayWhoosh()
{
    if (m_audio != nullptr && m_whoosh != nullptr)
    {
        m_audio->Play(m_whoosh);
    }
}

void GameController::HandleInput(SDL_Event _event)
{
    if (_event.type == SDL_EVENT_QUIT)
    {
        m_quit = true;
        return;
    }

    if (_event.type == SDL_EVENT_KEY_DOWN)
    {
        if (_event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_quit = true;
        }
        else if (_event.key.scancode == SDL_SCANCODE_I)
        {
            m_level->IncreaseEnemySpeed();
        }
        else if (_event.key.scancode == SDL_SCANCODE_D)
        {
            m_level->DecreaseEnemySpeed();
        }
        else if (_event.key.scancode == SDL_SCANCODE_S)
        {
            m_level->Save();
        }
        else if (_event.key.scancode == SDL_SCANCODE_L)
        {
            m_level->Load();
        }
    }
}

void GameController::RunGame()
{
    Initialize();

    while (!m_quit)
    {
        Timing::Instance().Tick();

        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        m_level->Update(Timing::Instance().GetDeltaTime());

        if (m_level->HasNoEnemiesLeft())
        {
            m_quit = true;
        }

        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        m_level->Render();

        m_font->Write(
            m_renderer->GetRenderer(),
            "[D]crease speed  [I]ncrease speed  [S]ave  [L]oad  [ESC] Quit",
            { 0, 0, 255 },
            { 10, 10 }
        );

        std::string playerSpeedText = "Player speed: " + std::to_string((int)m_level->GetPlayerSpeed());
        m_font->Write(
            m_renderer->GetRenderer(),
            playerSpeedText.c_str(),
            { 0, 150, 255 },
            { 10, 35 }
        );

        std::string enemySpeedText = "Enemy speed: " + std::to_string((int)m_level->GetEnemySpeed());
        m_font->Write(
            m_renderer->GetRenderer(),
            enemySpeedText.c_str(),
            { 0, 200, 0 },
            { 10, 60 }
        );

        std::string taggedText = "Enemies tagged: " + std::to_string(m_level->GetEnemiesTagged());
        m_font->Write(
            m_renderer->GetRenderer(),
            taggedText.c_str(),
            { 0, 255, 0 },
            { 10, 85 }
        );

        SDL_RenderPresent(m_renderer->GetRenderer());

        Timing::Instance().CapFPS();
    }

    ShutDown();
}
