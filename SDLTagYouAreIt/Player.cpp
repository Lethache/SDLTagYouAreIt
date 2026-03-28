#include "Player.h"
#include <cmath>

Player::Player()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_speed = 100.0f;
    m_scale = 1.25f;

    m_isMoving = false;
    m_facingRight = true;

    m_animState = ANIM_IDLE;

    m_frameIndex = 0;
    m_frameTimer = 0.0f;
    m_frameDuration = 0.12f;

    m_rows = 17;
    m_columns = 6;
    m_clipWidth = 69;
    m_clipHeight = 44;

    m_idleStart = 0;
    m_idleCount = 6;

    m_runStart = 6;
    m_runCount = 8;
}

Player::~Player()
{
    Shutdown();
}

bool Player::Initialize(SDL_Point _windowSize)
{
    m_texture.Load("./Assets/Textures/Warrior.tga");

    m_x = _windowSize.x * 0.5f;
    m_y = _windowSize.y * 0.5f;

    return true;
}

void Player::Update(float _dt)
{
    UpdateMovement(_dt);
    UpdateAnimation(_dt);
}

void Player::UpdateMovement(float _dt)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    float dirX = 0.0f;
    float dirY = 0.0f;

    if (keys[SDL_SCANCODE_LEFT])
    {
        dirX -= 1.0f;
        m_facingRight = false;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        dirX += 1.0f;
        m_facingRight = true;
    }
    if (keys[SDL_SCANCODE_UP])
    {
        dirY -= 1.0f;
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        dirY += 1.0f;
    }

    float length = std::sqrt(dirX * dirX + dirY * dirY);

    if (length > 0.0f)
    {
        dirX /= length;
        dirY /= length;

        m_x += dirX * m_speed * _dt;
        m_y += dirY * m_speed * _dt;

        m_isMoving = true;
        m_animState = ANIM_RUN;
    }
    else
    {
        m_isMoving = false;
        m_animState = ANIM_IDLE;
    }
}

void Player::UpdateAnimation(float _dt)
{
    m_frameTimer += _dt;

    if (m_frameTimer < m_frameDuration)
    {
        return;
    }

    m_frameTimer = 0.0f;

    int frameCount = (m_animState == ANIM_RUN) ? m_runCount : m_idleCount;
    m_frameIndex++;

    if (m_frameIndex >= frameCount)
    {
        m_frameIndex = 0;
    }
}

SDL_FRect Player::GetSourceRect() const
{
    int clipIndex = 0;

    if (m_animState == ANIM_RUN)
    {
        clipIndex = m_runStart + m_frameIndex;
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

void Player::Render()
{
    Renderer* renderer = &Renderer::Instance();

    SDL_FRect src = GetSourceRect();

    SDL_FRect dest;
    dest.w = m_clipWidth * m_scale;
    dest.h = m_clipHeight * m_scale;
    dest.x = m_x - dest.w * 0.5f;
    dest.y = m_y - dest.h * 0.5f;

    SDL_Texture* sdlTexture = renderer->GetSDLTexture(&m_texture);

    SDL_SetTextureColorMod(sdlTexture, 80, 80, 255);

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

void Player::Shutdown()
{
}