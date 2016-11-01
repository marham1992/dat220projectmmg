#ifndef CLIENT_TEXTBOX_H
#define CLIENT_TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using MessageContainer = std::vector<std::string>;

class Textbox {
public:
    Textbox();
    Textbox(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos);
    ~Textbox();

    void Render(sf::RenderWindow& l_wind);

    void Setup(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos);
    void Add(std::string l_message);
    void Clear();

private:
    MessageContainer m_messages;
    int m_numVisible;

    sf::RectangleShape m_backdrop;
    sf::Font m_font;
    sf::Text m_content;
};

#endif
