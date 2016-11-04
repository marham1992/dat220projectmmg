

#ifndef CLIENT_WITHCHATBOX_STATE_CHATTEST_H
#define CLIENT_WITHCHATBOX_STATE_CHATTEST_H


#include "BaseState.h"
#include "Client.h"
#include "Textbox.h"
#include "TextInputArea.h"
#include "EventManager.h"

class State_ChatTest : public BaseState {
public:
    State_ChatTest(StateManager* l_stateManager);
    ~State_ChatTest();

    void OnCreate();
    void OnDestroy();

    void Update(const sf::Time& l_time);
    void Draw();

    void Activate();
    void Deactivate();

    void HandlePacket(const PacketID& l_id, sf::Packet& l_packet, Client* l_client);

    void CheckInputArea(EventDetails* l_details);
    void GetTextEntered(EventDetails* l_details);

private:
    Client* m_client;
    TextInputArea* m_textInputArea;
    Textbox* m_textbox;

    void ActivateInput();
    void DeactivateInput();
    void SetupTextbox_and_InputArea();
};


#endif //CLIENT_WITHCHATBOX_STATE_CHATTEST_H
