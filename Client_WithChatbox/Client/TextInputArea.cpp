// TextInputArea.cpp --

#include "TextInputArea.h"


TextInputArea::TextInputArea(Client* l_client){
    Setup(3, 9, 200, sf::Vector2f(0,300), l_client);
}

TextInputArea::TextInputArea(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos, Client* l_client){
    Setup(l_visibleLines, l_charSize, l_width, l_screenPos, l_client);
}

TextInputArea::~TextInputArea(){
    Clear();
}

void TextInputArea::Setup(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos, Client* l_client){
    sf::Vector2f textOffset(2.0f, 2.0f);
    m_font.loadFromFile("Client/arial.ttf");

    m_client = l_client;
    m_numVisible = l_visibleLines;

    m_inputText.setFont(m_font);
    m_inputText.setString("");
    m_inputText.setCharacterSize(l_charSize);
    m_inputText.setFillColor(sf::Color::Magenta);
    m_inputText.setPosition(l_screenPos + textOffset);

    m_backdrop.setSize(sf::Vector2f(l_width, (l_visibleLines * (l_charSize*1.2f))));
    m_backdrop.setFillColor(sf::Color::White);
    m_backdrop.setOutlineColor(sf::Color::White);
    m_backdrop.setOutlineThickness(2);
    m_backdrop.setPosition(l_screenPos);
}

void TextInputArea::Clear() {
    //TODO: Clear m_inputString here?
    m_inputString.clear();
}
void TextInputArea::Render(sf::RenderWindow& l_wind){
    l_wind.draw(m_backdrop);
    if (m_inputString != "") {
        m_inputText.setString(m_inputString);
        l_wind.draw(m_inputText);
    }
}

//void TextInputArea::GatherInput(sf::RenderWindow& l_wind){
//    std::cout << "TextInputArea::GatherInput has been called" << std::endl;
//    m_active = true;
//    m_backdrop.setOutlineColor(sf::Color::Red);
//    sf::Event event;
//
//    while (m_active && l_wind.pollEvent(event)){
//        std::cout << "TextInputArea::GatherInput while loop running" << std::endl;
//        if(event.type==sf::Event::TextEntered)
//            HandleEvent(event);
//    }
//}

//void TextInputArea::HandleEvent(sf::Event l_event) {
//    char code = static_cast<char>(l_event.text.unicode);
//
//    if (l_event.text.unicode == 13) // Enter clicked.
//        Send();
//    else if (code != '\b')
//        m_inputString.push_back(code);
//    else if(code == '\b') {
//        if (m_inputString.size()>0)
//            m_inputString.pop_back();
//    }
//}

void TextInputArea::HandleTextEntered(sf::Uint32 textEntered){
    char code = static_cast<char>(textEntered);

    if (textEntered == 13) // Enter clicked.
        Send();
    else if (code != '\b')
        m_inputString.push_back(code);
    else if(code == '\b') {
        if (m_inputString.size()>0)
            m_inputString.pop_back();
    }
}

void TextInputArea::Send(){
    sf::Packet packet;
    StampPacket(PacketType::Message, packet);
    packet << m_inputString;
    m_client->Send(packet);
    Clear();
}

void TextInputArea::Stop(){
    m_active = false;
    m_backdrop.setOutlineColor(sf::Color::White);
}

bool TextInputArea::IsActive(){return m_active;}

// Use this to determine if a mouse click is within the TextInputArea
sf::FloatRect TextInputArea::GetBounds(){ return m_backdrop.getGlobalBounds(); }