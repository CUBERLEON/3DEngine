#include "Input.h"

#include <cstring>
#include "../rendering/Window.h"

Input::Input(Window* window) : m_window(window)
{
    m_keyboardKeyStates = new int[KEY_LAST+1];
    m_keyboardPressedKeys = new bool[KEY_LAST + 1];
    m_keyboardReleasedKeys = new bool[KEY_LAST + 1];

    m_mouseButtonStates = new int[MOUSE_BUTTON_LAST + 1];
    m_mousePressedButtons = new bool[MOUSE_BUTTON_LAST + 1];
    m_mouseReleasedButtons = new bool[MOUSE_BUTTON_LAST + 1];

    memset(m_keyboardKeyStates, 0, sizeof(int) * (KEY_LAST + 1));
    memset(m_keyboardPressedKeys, 0, sizeof(bool) * (KEY_LAST + 1));
    memset(m_keyboardReleasedKeys, 0, sizeof(bool) * (KEY_LAST + 1));

    memset(m_mouseButtonStates, 0, sizeof(int) * (MOUSE_BUTTON_LAST + 1));
    memset(m_mousePressedButtons, 0, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
    memset(m_mouseReleasedButtons, 0, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
}

Input::~Input()
{
    delete[] m_keyboardKeyStates;
    delete[] m_keyboardPressedKeys;
    delete[] m_keyboardReleasedKeys;

    delete[] m_mouseButtonStates;
    delete[] m_mousePressedButtons;
    delete[] m_mouseReleasedButtons;
}

glm::vec2 Input::getMousePosition() const
{
    double posx, posy;
    glfwGetCursorPos(m_window->getGLFWwindow(), &posx, &posy);
    return glm::vec2((float)posx, (float)posy);
}

void Input::setMousePosition(const glm::vec2& pos) const
{
    glfwSetCursorPos(m_window->getGLFWwindow(), pos.x, pos.y);
}

void Input::setCursor(bool value) const
{
    glfwSetInputMode(m_window->getGLFWwindow(), GLFW_CURSOR, value ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void Input::update()
{
    for (int key = 0; key <= KEY_LAST; ++key)
    {
        int oldState = m_keyboardKeyStates[key];
        m_keyboardKeyStates[key] = glfwGetKey(m_window->getGLFWwindow(), key);
        m_keyboardPressedKeys[key] = oldState == RELEASED && m_keyboardKeyStates[key] == PRESSED;
        m_keyboardReleasedKeys[key] = oldState == PRESSED && m_keyboardKeyStates[key] == RELEASED;
    }
    for (int button = 0; button <= MOUSE_BUTTON_LAST; ++button)
    {
        int oldState = m_mouseButtonStates[button];
        m_mouseButtonStates[button] = glfwGetMouseButton(m_window->getGLFWwindow(), button);
        m_mousePressedButtons[button] = oldState == RELEASED && m_mouseButtonStates[button] == PRESSED;
        m_mouseReleasedButtons[button] = oldState == PRESSED && m_mouseButtonStates[button] == RELEASED;
    }
}
