#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"


InputController::InputController()
{
    m_keyboard = new Keyboard();
    m_mouse = new Mouse();
    
}

InputController::~InputController()
{
    if (m_keyboard != nullptr)
    {
        delete m_keyboard;
        m_keyboard = nullptr;
    }
    if (m_mouse != nullptr)
    {
        delete m_mouse;
        m_mouse = nullptr;
    }
    if (m_gamepad != nullptr)
    {
        delete m_gamepad;
        m_gamepad = nullptr;
    }
}

void InputController::Initialize(SDL_Window* _window)
{
    m_keyboard->Initialize(_window);
   
}

void InputController::Process()
{
    m_mouse->Process();
}