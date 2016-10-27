

#ifndef CHATINPUTTEST_TEXTBOX_H
#define CHATINPUTTEST_TEXTBOX_H

#pragma once

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include <functional>

namespace sf
{
    class Packet;
}
class TextBox
{
private:
    const std::size_t historyLength;
    const float thickness;
    const int charSize;
    const float length;

    sf::RectangleShape box1;  //histories go here


    sf::Text historyText;
    std::deque<std::string> history;


    std::function<void(const std::string & s)> onEnter;
public:
    TextBox(const sf::Vector2f pos,const float length, const float thickness, const int charSize, const std::size_t historyLength, const sf::Font & font);
    ~TextBox();

    void setOutlineColor(const sf::Color & color);
    void setFillColor(const sf::Color & color);
    void setCharColor(const sf::Color & color);


    void handleEvent(sf::Event & event,sf::RenderWindow & window);

    void push(const std::string & s);
    void connectOnEnter(std::function<void(const std::string & s)> func);

    void draw(sf::RenderWindow & window);

};


#endif //CHATINPUTTEST_TEXTBOX_H
