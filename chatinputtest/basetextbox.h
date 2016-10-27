//
// Created by martin on 26.10.16.
//

#ifndef CHATINPUTTEST_BASETEXTBOX_H
#define CHATINPUTTEST_BASETEXTBOX_H

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include <functional>

class BaseTextBox {
private:
//    const std::size_t historyLength;
    const float thickness;
    const int charSize;
    const float length;

/*
    sf::RectangleShape box1;  //histories go here
    sf::RectangleShape box2;  //inputString goes here
*/


    std::function<void(const std::string & s)> onEnter;
public:
    BaseTextBox(const sf::Vector2f pos,const float length, const float thickness, const int charSize, const std::size_t historyLength, const sf::Font & font);
    ~BaseTextBox();

    void setOutlineColor(const sf::Color & color);
    void setFillColor(const sf::Color & color);
    void setCharColor(const sf::Color & color);



    void handleEvent(sf::Event & event);

    void draw(sf::RenderWindow & window);
};


#endif //CHATINPUTTEST_BASETEXTBOX_H
