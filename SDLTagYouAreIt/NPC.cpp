#include "NPC.h"
#include "GameController.h"
#include <cmath>

NPC::NPC()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_speed = 60.0f;
    m_scale = 1.25f;

    m_isMoving = false;
    m_facingRight = true;
    m_animState = ANIM_IDLE;

    m_tagged = false;
    m_deadFinished = false;

    m_frameIndex = 0;
    m_frameTimer = 0.0f;
    m_frameDuration = 0.12f;

    m_columns = 6;
    m_clipWidth = 69;
    m_clipHeight = 44;

    m_idleStart = 0;
    m_idleCount = 6;

    m_runStart = 6;
    m_runCount = 8;

    m_deathStart = 26;
    m_deathCount = 11;
}

NPC::~NPC()
{
    Shutdown();
}

void NPC::Reset()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_speed = 60.0f;
    m_scale = 1.25f;

    m_isMoving = false;
    m_facingRight = true;
    m_animState = ANIM_IDLE;

    m_tagged = false;
    m_deadFinished = false;

    m_frameIndex = 0;
    m_frameTimer = 0.0f;
}

bool NPC::Initialize(float _x, float _y)
{
    m_texture.Load("./Assets/Textures/Warrior.tga");
    m_x = _x;
    m_y = _y;

    m_isMoving = false;
    m_animState = ANIM_IDLE;
    m_tagged = false;
    m_deadFinished = false;
    m_frameIndex = 0;
    m_frameTimer = 0.0f;

    return true;
}

void NPC::Update(float _dt, float _playerX, float _playerY)
{
    if (m_deadFinished)
    {
        return;
    }

    float dirX = _playerX - m_x;
    float dirY = _playerY - m_y;

    float distance = std::sqrt(dirX * dirX + dirY * dirY);

    m_facingRight = (dirX >= 0.0f);

    if (!m_tagged && distance < 30.0f)
    {
        m_tagged = true;
        m_animState = ANIM_DEATH;
        m_frameIndex = 0;
        m_frameTimer = 0.0f;

        GameController::Instance().PlayWhoosh();
    }

    if (!m_tagged)
    {
        m_isMoving = false;
        m_animState = ANIM_IDLE;

        if (distance > 0.0f)
        {
            dirX /= distance;
            dirY /= distance;

            if (distance < 140.0f)
            {
                m_x -= dirX * m_speed * _dt;
                m_y -= dirY * m_speed * _dt;
                m_isMoving = true;
                m_animState = ANIM_RUN;
            }
            else if (distance > 160.0f)
            {
                m_x += dirX * m_speed * _dt;
                m_y += dirY * m_speed * _dt;
                m_isMoving = true;
                m_animState = ANIM_RUN;
            }
        }
    }

    UpdateAnimation(_dt);
}

void NPC::UpdateAnimation(float _dt)
{
    m_frameTimer += _dt;

    if (m_frameTimer < m_frameDuration)
    {
        return;
    }

    m_frameTimer = 0.0f;

    if (m_animState == ANIM_DEATH)
    {
        m_frameIndex++;

        if (m_frameIndex >= m_deathCount)
        {
            m_frameIndex = m_deathCount - 1;
            m_deadFinished = true;
        }

        return;
    }

    int frameCount = (m_animState == ANIM_RUN) ? m_runCount : m_idleCount;

    m_frameIndex++;

    if (m_frameIndex >= frameCount)
    {
        m_frameIndex = 0;
    }
}

SDL_FRect NPC::GetSourceRect() const
{
    int clipIndex = 0;

    if (m_animState == ANIM_RUN)
    {
        clipIndex = m_runStart + m_frameIndex;
    }
    else if (m_animState == ANIM_DEATH)
    {
        clipIndex = m_deathStart + m_frameIndex;
    }
    else
    {
        clipIndex = m_idleStart + m_frameIndex;
    }

    int clipX = (clipIndex % m_columns) * m_clipWidth;
    int clipY = (clipIndex / m_columns) * m_clipHeight;

    SDL_FRect src;
    src.x = (float)clipX;
    src.y = (float)clipY;
    src.w = (float)m_clipWidth;
    src.h = (float)m_clipHeight;

    return src;
}

void NPC::Render()
{
    if (m_deadFinished)
    {
        return;
    }

    Renderer* renderer = &Renderer::Instance();

    SDL_FRect src = GetSourceRect();

    SDL_FRect dest;
    dest.w = m_clipWidth * m_scale;
    dest.h = m_clipHeight * m_scale;
    dest.x = m_x - dest.w * 0.5f;
    dest.y = m_y - dest.h * 0.5f;

    SDL_Texture* sdlTexture = renderer->GetSDLTexture(&m_texture);

    if (m_tagged)
        SDL_SetTextureColorMod(sdlTexture, 255, 0, 0);
    else
        SDL_SetTextureColorMod(sdlTexture, 0, 220, 0);

    src.y = m_texture.GetImageInfo()->Height - src.y - src.h;

    SDL_FlipMode flip = SDL_FLIP_VERTICAL;
    if (!m_facingRight)
    {
        flip = (SDL_FlipMode)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
    }

    SDL_RenderTextureRotated(
        renderer->GetRenderer(),
        sdlTexture,
        &src,
        &dest,
        0.0,
        nullptr,
        flip
    );
}

void NPC::Shutdown()
{
}