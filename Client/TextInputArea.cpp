#include "TextInputArea.h"
#include <iostream>


TextInputArea::TextInputArea(const sf::Vector2f pos, const float length_, const float thickness_,
                             const int charSize_, const sf::Font & font):
        thickness(thickness_), charSize(charSize_), length(length_),
        box2(sf::Vector2f(length, thickness*2+charSize))
{
    box2.setPosition(pos);
    /*float x2=box1.getGlobalBounds().left;
    float y2=box1.getGlobalBounds().top + box1.getGlobalBounds().height+3*thickness;*/

    box2.setOutlineThickness(thickness);

    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setOutlineColor(sf::Color::Black);
    text.setColor(sf::Color::Black);
    text.setPosition(pos.x+thickness, pos.y+thickness);

}

TextInputArea::~TextInputArea(void)
{
}

void TextInputArea::update()
{
    text.setString(inputString);
}

void TextInputArea::send(const std::string & s) //sends information
{
    if(s.size()>0)
    {
        //Displays text on screen
        std::cout << s << std::endl;
        //TODO: Send packet to server

    }
}

void TextInputArea::connectOnEnter(std::function<void(const std::string & s)> func)
{
    onEnter=func;
}

void TextInputArea::draw(sf::RenderWindow & window)
{
    window.draw(box2);
    window.draw(text);


}


void TextInputArea::setOutlineColor(const sf::Color & color)
{
    box2.setOutlineColor(color);
}

void TextInputArea::setFillColor(const sf::Color & color)
{
    box2.setFillColor(color);
}

void TextInputArea::setCharColor(const sf::Color & color)
{
    text.setColor(color);
}

bool TextInputArea::isActive(){
    return active;
}

sf::FloatRect TextInputArea::GetBounds(){
    return box2.getGlobalBounds();
}

void TextInputArea::HandleTextEntered(sf::Uint32 textEntered) {
    char code = static_cast<char>(textEntered);
    if(textEntered == 13){
        //enter clicked
    }
    else if (code != '\b'){
        inputString.push_back(code);
    }
    else if (code == '\b'){
        if (inputString.size()>0)
            inputString.pop_back();
    }
}

void TextInputArea::Stop() {
    active = false;
    box2.setOutlineColor(sf::Color::White);
}

void TextInputArea::Activate() {
    box2.setOutlineColor(sf::Color::Green);
    active = true;
}
