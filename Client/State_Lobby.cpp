#include "State_Lobby.h"
#include "StateManager.h"

State_Lobby::State_Lobby(StateManager* l_stateManager)
        : BaseState(l_stateManager){
    m_font.loadFromFile("arial.ttf");
    m_chatBox = new ChatBox(sf::Vector2f(50,50), 200, 3,16, 10, m_font);
    //ChatBox(const sf::Vector2f pos, const float length_, const float thickness_,
    //const int charSize_, const std::size_t historyLength_, const sf::Font & font)
}

State_Lobby::~State_Lobby(){}

void State_Lobby::OnCreate(){

    m_text.setFont(m_font);
    m_text.setString(sf::String("Lobby:"));
    m_text.setCharacterSize(18);

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
                     textRect.top + textRect.height / 2.0f);

    m_text.setPosition(400,10);

    m_buttonSize = sf::Vector2f(300.0f,32.0f);
    m_buttonPos = sf::Vector2f(m_stateMgr->GetContext()->m_wind->GetWindowSize().x-m_buttonSize.x,100);
    m_buttonPadding = 4; // 4px.

    std::string str[4];
    str[0] = "Join Game";
    str[1] = "Create New Game";
    str[2] = "CREDITS";
    str[3] = "EXIT";

    for(int i = 0; i < 4; ++i){
        sf::Vector2f buttonPosition(m_buttonPos.x,m_buttonPos.y +
                                                  (i * (m_buttonSize.y + m_buttonPadding)));
        m_rects[i].setSize(m_buttonSize);
        m_rects[i].setFillColor(sf::Color::Red);

        m_rects[i].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
        m_rects[i].setPosition(buttonPosition);

        m_labels[i].setFont(m_font);
        m_labels[i].setString(sf::String(str[i]));
        m_labels[i].setCharacterSize(12);

        sf::FloatRect rect = m_labels[i].getLocalBounds();
        m_labels[i].setOrigin(rect.left + rect.width / 2.0f,
                              rect.top + rect.height / 2.0f);

        m_labels[i].setPosition(buttonPosition);
    }

    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::Lobby,"Mouse_Left",&State_Lobby::MouseClick,this);
}

void State_Lobby::OnDestroy(){
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::Lobby,"Mouse_Left");
}

void State_Lobby::Activate(){
    if((m_stateMgr->HasState(StateType::Game)) && (m_labels[0].getString() == "TWO PLAYER"))
    {
        m_labels[0].setString(sf::String("RESUME TWO PLAYER"));
        sf::FloatRect rect = m_labels[0].getLocalBounds();
        m_labels[0].setOrigin(rect.left + rect.width / 2.0f,
                              rect.top + rect.height / 2.0f);
    }
    if((m_stateMgr->HasState(StateType::GameVsAi)) && (m_labels[1].getString() == "VS AI"))
    {
        m_labels[1].setString(sf::String("RESUME VS AI"));
        sf::FloatRect rect = m_labels[1].getLocalBounds();
        m_labels[1].setOrigin(rect.left + rect.width / 2.0f,
                              rect.top + rect.height / 2.0f);
    }
}

void State_Lobby::MouseClick(EventDetails* l_details){
    sf::Vector2i mousePos = l_details->m_mouse;

    float halfX = m_buttonSize.x / 2.0f;
    float halfY = m_buttonSize.y / 2.0f;
    for(int i = 0; i < 4; ++i){
        if(mousePos.x >= m_rects[i].getPosition().x - halfX &&
           mousePos.x <= m_rects[i].getPosition().x + halfX &&
           mousePos.y >= m_rects[i].getPosition().y - halfY &&
           mousePos.y <= m_rects[i].getPosition().y + halfY)
        {
            if(i == 0){
                //m_stateMgr->SwitchTo(StateType::Game);
            } else if(i == 1){
                //m_stateMgr->SwitchTo(StateType::GameVsAi);
            } else if(i == 2){
                // Credits state.
            } else if(i == 3){
                m_stateMgr->GetContext()->m_wind->Close();
            }
        }
    }
}

void State_Lobby::Draw(){
    sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
    window->draw(m_text);
    for(int i = 0; i < 4; ++i){
        window->draw(m_rects[i]);
        window->draw(m_labels[i]);
    }
    m_chatBox->draw(*window);
}

void State_Lobby::Update(const sf::Time& l_time){}
void State_Lobby::Deactivate(){}
