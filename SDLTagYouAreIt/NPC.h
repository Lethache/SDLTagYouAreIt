#ifndef NPC_H
#define NPC_H

#include "StandartIncludes.h"
#include "Texture.h"
#include "Renderer.h"

class NPC
{
public:
    NPC();
    ~NPC();

    bool Initialize(float _x, float _y);
    void Update(float _dt, float _playerX, float _playerY);
    void Render();
    void Shutdown();
    void Reset();

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    void SetPosition(float _x, float _y) { m_x = _x; m_y = _y; }
    void SetSpeed(float _speed) { m_speed = _speed; }

    bool IsTagged() const { return m_tagged; }
    bool IsDeadFinished() const { return m_deadFinished; }

private:
    enum AnimState
    {
        ANIM_IDLE = 0,
        ANIM_RUN,
        ANIM_DEATH
    };

    SDL_FRect GetSourceRect() const;
    void UpdateAnimation(float _dt);

private:
    Texture m_texture;

    float m_x;
    float m_y;
    float m_speed;
    float m_scale;

    bool m_isMoving;
    bool m_facingRight;

    bool m_tagged;
    bool m_deadFinished;

    AnimState m_animState;

    int m_frameIndex;
    float m_frameTimer;
    float m_frameDuration;

    int m_columns;
    int m_clipWidth;
    int m_clipHeight;

    int m_idleStart;
    int m_idleCount;

    int m_runStart;
    int m_runCount;

    int m_deathStart;
    int m_deathCount;
};

#endif