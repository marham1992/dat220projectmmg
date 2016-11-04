// Because the StateManager class is forwardly declared in the BaseState header,
// we MUST include the StateManager header in the implementation file -- true for any state.
#include "State_Intro.h"
#include "StateManager.h"

State_Intro::State_Intro(StateManager* l_stateManager)
        : BaseState(l_stateManager){
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();
    float inputareaWidth = 100;
    m_inputIP = new TextInputArea(sf::Vector2f( (windowSize.x / 2) - (inputareaWidth/2) , (windowSize.y / 2) + 50 ),inputareaWidth, 5, 15, m_font);
}

State_Intro::~State_Intro(){
    delete m_inputIP;
    m_inputIP = nullptr;
}

void State_Intro::OnCreate(){
    m_timePassed = 0.0f;

    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();

    m_introTexture.loadFromFile("intro.png");
    m_introSprite.setTexture(m_introTexture);
    m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f,
                            m_introTexture.getSize().y / 2.0f);

    m_introSprite.setPosition(windowSize.x / 2.0f, 0);

    // ****** FONT FILE HERE *****
    m_font.loadFromFile("arial.ttf");
    m_text.setFont(m_font);
    m_text.setString({ "Enter IP" });
    m_text.setCharacterSize(15);
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
                     textRect.top + textRect.height / 2.0f);
    m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);



    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::Intro,"Intro_Continue",&State_Intro::Continue,this);
    evMgr->AddCallback(StateType::Intro,"Mouse_Left",&State_Intro::MouseClick,this);
}

void State_Intro::OnDestroy(){
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::Intro,"Intro_Continue");
}

// Because we're dealing with time and movement here, updating the state is necessary.
void State_Intro::Update(const sf::Time& l_time){
    if(m_timePassed < 5.0f){ // Less than five seconds.
        m_timePassed += l_time.asSeconds();
        m_introSprite.setPosition(
                m_introSprite.getPosition().x,
                m_introSprite.getPosition().y + (48 * l_time.asSeconds()));
    }
    m_inputIP->update();
}

void State_Intro::Draw(){
    sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

    // Telling the window to draw what we want.
    window->draw(m_introSprite);
    m_inputIP->draw(*window);
    if(m_timePassed >= 5.0f){
        window->draw(m_text);
    }
}

void State_Intro::Continue(EventDetails* l_details){
    // Changed time req to small number, from 5.
    if(m_timePassed >= 0.5f){
        m_stateMgr->SwitchTo(StateType::MainMenu);
        // Removes 'itself' from the stack since we will not need the intro state again.
        m_stateMgr->Remove(StateType::Intro);
    }
}

void State_Intro::MouseClick(EventDetails * l_details) {
    sf::Vector2f mousePos(l_details->m_mouse.x,l_details->m_mouse.y);
    bool Active = m_inputIP->isActive();
    bool WithinArea = m_inputIP->GetBounds().contains(mousePos);

    if(WithinArea && !Active){
        ActivateInput();
    }
    else if (WithinArea && Active){
        DeactivateInput();
    }
}

void State_Intro::Activate(){}
void State_Intro::Deactivate(){}

void State_Intro::ActivateInput() {
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->AddCallback(StateType::Intro, "Text_Entered", &State_Intro::GetTextEntered, this);
    m_inputIP->Activate();
}

void State_Intro::DeactivateInput() {
    m_inputIP->Stop();
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
    evMgr->RemoveCallback(StateType::Intro, "Text_Entered");
}

void State_Intro::GetTextEntered(EventDetails* l_details) {
    m_inputIP->HandleTextEntered(l_details->m_textEntered);
}
