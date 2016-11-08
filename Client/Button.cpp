#include "Button.h"

Button::Button(std::string text, const sf::Vector2f pos,float width, const int charSize_, const sf::Font &font_) : m_charSize(charSize_) {
    m_text.setCharacterSize(m_charSize);
    m_text.setFont(font_);
    m_text.setString(text);
    m_background.setFillColor(sf::Color::Green);
    m_background.setSize(sf::Vector2f(width,charSize_+2*m_padding));
    m_background.setPosition(pos.x-(width/2),pos.y);
    m_text.setPosition(sf::Vector2f(m_background.getPosition().x+m_padding,pos.y+m_padding/2));
    m_text.setFillColor(sf::Color::Black);
}

Button::~Button(void) {

}

void Button::update()
{

}

void Button::draw(sf::RenderWindow & window)
{
    window.draw(m_background);
    window.draw(m_text);
}

sf::FloatRect Button::GetBounds(){
    return m_background.getGlobalBounds();
}