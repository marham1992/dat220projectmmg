
#include "textinputarea.h"
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
    text.setPosition(pos.x+thickness, pos.y+thickness);




}

TextInputArea::~TextInputArea(void)
{
}

void TextInputArea::update()
{
    text.setString(inputString);
}
/**
 * window needed for handling MousePos relative to the renderwindow
 *
 * @param event
 * @param window
 */
void TextInputArea::handleEvent(sf::Event & event, sf::RenderWindow & window)
{
    if(active) // textentry is active
    {
        if(event.type==sf::Event::TextEntered)
        {
            char code=static_cast<char>(event.text.unicode);
            //TODO: Test these on different OS's
            if(event.text.unicode==13)//enter
            {
//            onEnter(inputString);
                send(inputString);
                inputString.clear();
            }

            else if(code!='\b')
                inputString.push_back(code);
            else if(code=='\b') // equals backspace
            {
                if(inputString.size()>0)
                    inputString.pop_back();
            }
        }
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {

        if(box2.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y))
        {
            active = true;
            setOutlineColor(sf::Color::Green);
        }
        else
        {
            active = false;
            setOutlineColor(sf::Color::Yellow);
        }
    }
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