#include "TTFont.h"

TTFont::TTFont()
{
    m_font = nullptr;
}

TTFont::~TTFont()
{
    Shutdown();
}

void TTFont::Initialize(int _pointSize)
{
    M_ASSERT(TTF_Init(), "Unable to initialize SDL_ttf.");

    // Если Assets лежит в x64\Debug\Assets
    m_font = TTF_OpenFont("./Assets/Fonts/arial.ttf", (float)_pointSize);

    M_ASSERT(m_font != nullptr, "Failed to load font.");
}

void TTFont::Shutdown()
{
    if (m_font != nullptr)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }

    TTF_Quit();
}

void TTFont::Write(SDL_Renderer* _renderer, const char* _text, SDL_Color _color, SDL_Point _pos)
{
    if (_text == nullptr || strlen(_text) == 0)
    {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(m_font, _text, 0, _color);
    if (surface == nullptr)
    {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (texture == nullptr)
    {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect destRect;
    destRect.x = (float)_pos.x;
    destRect.y = (float)_pos.y;
    destRect.w = (float)surface->w;
    destRect.h = (float)surface->h;

    M_ASSERT(
        SDL_RenderTextureRotated(_renderer, texture, nullptr, &destRect, 0.0, nullptr, SDL_FLIP_NONE) == true,
        "Could not render font texture"
    );

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}