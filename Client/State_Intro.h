#ifndef CLIENT_STATE_INTRO_H
#define CLIENT_STATE_INTRO_H

#include "BaseState.h"
#include "EventManager.h"
#include "ChatBox.h"
#include "TextInputArea.h"

class State_Intro : public BaseState{
public:
    State_Intro(StateManager* l_stateManager);
    ~State_Intro();

    void OnCreate();
    void OnDestroy();

    void ActivateInput();
    void DeactivateInput();

    void Activate();
    void Deactivate();

    void Update(const sf::Time& l_time);
    void Draw();

    void Continue(EventDetails* l_details);
private:
    sf::Texture m_introTexture;
    sf::Sprite m_introSprite;
    sf::Font m_font;
    sf::Text m_text;
    TextInputArea* m_inputIP;


    // Keeps track of time spent in this state.
    float m_timePassed;

    void MouseClick(EventDetails *);

    void GetTextEntered(EventDetails *);
};

#endif
