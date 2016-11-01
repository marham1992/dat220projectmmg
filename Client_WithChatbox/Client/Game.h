
#ifndef CLIENT_WITHCHATBOX_GAME_H
#define CLIENT_WITHCHATBOX_GAME_H


#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
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
    void RestartClock();
    sf::Clock m_clock;
    sf::Time m_elapsed;

    SharedContext m_context;
    Window m_window;
    StateManager m_stateManager;
    Client m_client;
};


#endif //CLIENT_WITHCHATBOX_GAME_H
