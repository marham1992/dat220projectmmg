#ifndef CLIENT_TEXTINPUTAREA_H
#define CLIENT_TEXTINPUTAREA_H

#pragma once

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include "EventManager.h"
#include "SharedContext.h"
#include <functional>

namespace sf
{
    class Packet;
}

class TextInputArea
{
private:

    const float thickness;
    const int charSize;
    const float length;
    bool active;


    sf::RectangleShape box2;  //inputString goes here

    sf::Text text;


    std::string inputString;

    std::function<void(const std::string & s)> onEnter;

public:
    TextInputArea(const sf::Vector2f pos,const float length, const float thickness, const int charSize, const sf::Font & font);
    ~TextInputArea();

    void setOutlineColor(const sf::Color & color);
    void setFillColor(const sf::Color & color);
    void setCharColor(const sf::Color & color);
    void setPosition(sf::Vector2f pos);

    bool isActive();

    sf::FloatRect GetBounds();

    void Stop();

    void Activate();



    void update();
    void handleMouseClick(sf::Vector2f l_mousePos);

    void send(const std::string & s);
    void connectOnEnter(std::function<void(const std::string & s)> func);

    void draw(sf::RenderWindow & window);

    void HandleTextEntered(sf::Uint32 textEntered);
};

#endif
