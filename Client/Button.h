#ifndef CLIENT_BUTTON_H
#define CLIENT_BUTTON_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Button {
private:

    int m_charSize;
    int m_padding = 5;
    sf::RectangleShape m_background;
    sf::Text m_text;

    std::function<void(const std::string & s)> onEnter;

public:
    Button(std::string text, const sf::Vector2f pos,float width, const int charSize, const sf::Font & font);
    ~Button(void);

    void setOutlineColor(const sf::Color & color);
    void setFillColor(const sf::Color & color);
    void setCharColor(const sf::Color & color);

    sf::FloatRect GetBounds();

    void Stop();

    void Activate();



    void update();
    void handleMouseClick(sf::Vector2f l_mousePos);

    void connectOnEnter(std::function<void(const std::string & s)> func);

    void draw(sf::RenderWindow & window);
};


#endif
