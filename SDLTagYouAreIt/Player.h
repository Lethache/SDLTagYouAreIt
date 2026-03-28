#ifndef PLAYER_H
#define PLAYER_H

#include "StandartIncludes.h"
#include "Texture.h"
#include "Renderer.h"

class Player
{
public:
    Player();
    ~Player();

    bool Initialize(SDL_Point _windowSize);
    void Update(float _dt);
    void Render();
    void Shutdown();

    float GetSpeed() const { return m_speed; }
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    void SetPosition(float _x, float _y) { m_x = _x; m_y = _y; }

private:
    enum AnimState
    {
        ANIM_IDLE = 0,
        ANIM_RUN
    };

    void UpdateAnimation(float _dt);
    void UpdateMovement(float _dt);
    SDL_FRect GetSourceRect() const;

private:
    Texture m_texture;

    float m_x;
    float m_y;
    float m_speed;
    float m_scale;

    bool m_isMoving;
    bool m_facingRight;

    AnimState m_animState;

    int m_frameIndex;
    float m_frameTimer;
    float m_frameDuration;

    int m_rows;
    int m_columns;
    int m_clipWidth;
    int m_clipHeight;

    int m_idleStart;
    int m_idleCount;

    int m_runStart;
    int m_runCount;
};

#endif