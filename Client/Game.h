#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "Textbox.h"
#include <iostream>


class Game{
public:
    Game();
    ~Game();

    void Update();
    void Render();
    void LateUpdate();

    sf::Time GetElapsed();

    Window* GetWindow();
private:
    SharedContext m_context;
    Window m_window;
    StateManager m_stateManager;
    sf::Clock m_clock;
    sf::Time m_elapsed;
    void RestartClock();
};

#endif
