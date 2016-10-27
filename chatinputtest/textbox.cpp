
#include "textbox.h"
#include <iostream>


TextBox::TextBox(const sf::Vector2f pos, const float length_, const float thickness_,
                 const int charSize_, const std::size_t historyLength_, const sf::Font & font):
        thickness(thickness_), charSize(charSize_), historyLength(historyLength_), length(length_),
        box1(sf::Vector2f(length, thickness*(historyLength)+charSize*historyLength))

{
    box1.setPosition(pos);
    box1.setOutlineThickness(thickness);
    historyText.setFont(font);
    historyText.setCharacterSize(charSize);


}

TextBox::~TextBox(void)
{
}



void TextBox::handleEvent(sf::Event & event,sf::RenderWindow & window)
{
    //TODO: write events. Possible mouseclicks if the window is to be clickable(open_games_list)
    /*if(event.type==sf::Event::TextEntered)
    {
        char code=static_cast<char>(event.text.unicode);

        if(event.text.unicode==13)//enter
        {
//            onEnter(inputString);
            push(inputString);
            inputString.clear();
        }

        else if(code!='\b')
            inputString.push_back(code);
        else if(code=='\b')
        {
            if(inputString.size()>0)
                inputString.pop_back();
        }
    }*/

}

void TextBox::push(const std::string & s)
{
    if(s.size()>0)
    {
        history.push_front(s);
        if(history.size()>historyLength)
            history.pop_back();
    }
    //TODO: Get info from textinputarea or other sources(packets)

}

void TextBox::connectOnEnter(std::function<void(const std::string & s)> func)
{
    onEnter=func;
}

void TextBox::draw(sf::RenderWindow & window)
{
    window.draw(box1);

    float x=box1.getGlobalBounds().left+2*thickness;
    float y=box1.getGlobalBounds().top+box1.getGlobalBounds().height-thickness-charSize;

    for(std::size_t i=0; i<history.size(); ++i)
    {
        historyText.setString(history[i]);
        historyText.setPosition(x, y-i*charSize-i*thickness-thickness);
        window.draw(historyText);
    }
}


void TextBox::setOutlineColor(const sf::Color & color)
{
    box1.setOutlineColor(color);
}

void TextBox::setFillColor(const sf::Color & color)
{
    box1.setFillColor(color);
}

void TextBox::setCharColor(const sf::Color & color)
{
    historyText.setColor(color);
}

