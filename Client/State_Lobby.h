#ifndef CLIENT_STATE_MAINMENU_H
#define CLIENT_STATE_MAINMENU_H

#include "BaseState.h"
#include "EventManager.h"
#include "ChatBox.h"

class State_Lobby : public BaseState{
public:
    State_Lobby(StateManager* l_stateManager);
    ~State_Lobby();

    void OnCreate();
    void OnDestroy();

    void Activate();
    void Deactivate();

    void Update(const sf::Time& l_time);
    void Draw();

    void MouseClick(EventDetails* l_details);
private:
    sf::Font m_font;
    sf::Text m_text;

    sf::Vector2f m_buttonSize;
    sf::Vector2f m_buttonPos;
    unsigned int m_buttonPadding;

    sf::RectangleShape m_rects[4];
    sf::Text m_labels[4];

    ChatBox* m_chatBox;
};
#endif
