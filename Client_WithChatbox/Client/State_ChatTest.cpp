// State_ChatTest.cpp --

#include <iostream>
#include "State_ChatTest.h"
#include "StateManager.h"

State_ChatTest::State_ChatTest(StateManager* l_stateManager)
        : BaseState(l_stateManager), m_client(m_stateMgr->GetContext()->m_client){}

State_ChatTest::~State_ChatTest() {}

/// Server info from console.
//void GetServerInfoFromConsole(sf::IpAddress& l_ip, PortNumber& l_port){
//    std::cout << "Enter Server IP: ";
//    std::cin >> l_ip;
//    std::cout << "Enter Server Port: ";
//    std::cin >> l_port;
//}
/// Static server info.
void UseStaticServerInfo(sf::IpAddress& l_ip, PortNumber& l_port){
    l_ip = "www.weightmaker.com";
    l_port = 5600;
}

void State_ChatTest::OnCreate(){
    sf::IpAddress ip;   // Server IP
    PortNumber port;    // Server Port#

    /// Also uncomment one of the functions above.
//    GetServerInfoFromConsole(ip, port);
    UseStaticServerInfo(ip, port);

    m_client->SetServerInformation(ip, port);
    std::cout << "m_client->SetServerInformation is done." << std::endl;
    m_client->Setup(&State_ChatTest::HandlePacket, this);            // Register packet handler.
    std::cout << "m_client->Setup is done." << std::endl;

    if (m_client->Connect()){  // Try to connect
        std::cout << "if(m_client->Connect()) is true" << std::endl;
        sf::Clock clock;
        clock.restart();
    } else {
        std::cout << "Failed to connect." << std::endl;
    }

    SetupTextbox_and_InputArea();

    //TODO: Add callbacks.
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::ChatTest, "Mouse_Left", &State_ChatTest::CheckInputArea, this);
}

void State_ChatTest::OnDestroy() {
    m_client->Disconnect();
    m_client->UnregisterPacketHandler();

    delete m_textbox;
    m_textbox = nullptr;
    delete m_textInputArea;
    m_textInputArea = nullptr;

    //TODO: Remove callbacks.
//    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
//    evMgr->RemoveCallback(StateType::ChatTest, "SomeName");
}

void State_ChatTest::Update(const sf::Time& l_time) {
//    if (!m_client->IsConnected()){ m_stateMgr->Remove(StateType::Game); m_stateMgr->SwitchTo(StateType::MainMenu); return; }
}

void State_ChatTest::Draw() {
    m_textbox->Render(*m_stateMgr->GetContext()->m_wind->GetRenderWindow());
    m_textInputArea->Render(*m_stateMgr->GetContext()->m_wind->GetRenderWindow());
}

void State_ChatTest::Activate() {}
void State_ChatTest::Deactivate() {}

// Packet handler
void State_ChatTest::HandlePacket(const PacketID& l_id, sf::Packet& l_packet, Client* l_client){
    // The client responds to two types of packets: Messages and Disconnect.
    if ((PacketType)l_id == PacketType::Message){
        // Pass on message to m_textbox
        std::string message;
        l_packet >> message;
        m_textbox->Add(message);
    } else if ((PacketType)l_id == PacketType::Disconnect){
        m_client->Disconnect();
    }
}

void State_ChatTest::CheckInputArea(EventDetails *l_details) {
    sf::Vector2f mousePos(l_details->m_mouse.x, l_details->m_mouse.y);
    bool Active = m_textInputArea->IsActive();
    bool WithinArea = m_textInputArea->GetBounds().contains(mousePos);

    if (WithinArea && !Active) { ActivateInput(); }
    else if (!WithinArea && Active) { DeactivateInput(); }
}


void State_ChatTest::GetTextEntered(EventDetails* l_details){
    m_textInputArea->HandleTextEntered(l_details->m_textEntered);
}

void State_ChatTest::ActivateInput(){
    /// Remove callbacks that conflict with text input here:
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::ChatTest, "Text_Entered", &State_ChatTest::GetTextEntered, this);
}

void State_ChatTest::DeactivateInput(){
    m_textInputArea->Stop();
    /// Add previously removed callbacks that conflicted with text input here:
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::ChatTest, "Text_Entered");
}

void State_ChatTest::SetupTextbox_and_InputArea() {
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
    int textboxVisibleLines = 10;
    int charSize = 14;
    float textboxHeight = textboxVisibleLines*(charSize*1.2f);
    int textboxWidth = 250;

    m_textbox = new Textbox(10, charSize, textboxWidth, sf::Vector2f(windowSize.x/2-textboxWidth/2, 0));
    m_textInputArea = new TextInputArea(3, charSize, textboxWidth, sf::Vector2f(windowSize.x/2-textboxWidth/2, textboxHeight), m_client);
}