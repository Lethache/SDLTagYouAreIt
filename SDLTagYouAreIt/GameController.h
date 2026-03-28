#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandartIncludes.h"

class Renderer;
class Level;
class TTFont;
class AudioController;
class SoundEffect;

class GameController : public Singleton<GameController>
{
public:
    GameController();
    virtual ~GameController();

    void RunGame();
    void Initialize();
    void HandleInput(SDL_Event _event);
    void ShutDown();

    void PlayWhoosh();

private:
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    Level* m_level;
    TTFont* m_font;
    AudioController* m_audio;
    SoundEffect* m_whoosh;
    bool m_quit;
};

#endif