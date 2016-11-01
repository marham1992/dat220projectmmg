// TextInputArea.h --

#ifndef CLIENT_WITHCHATBOX_TEXTINPUTAREA_H
#define CLIENT_WITHCHATBOX_TEXTINPUTAREA_H

#include "SFML/Graphics.hpp"
#include "Client.h"


class TextInputArea {
public:
    TextInputArea(Client* l_client);
    TextInputArea(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos, Client* l_client);
    ~TextInputArea();

    void Setup(int l_visibleLines, int l_charSize, int l_width, sf::Vector2f l_screenPos, Client* l_client);
    void Clear();
    void Render(sf::RenderWindow& l_wind);

//    void GatherInput(sf::RenderWindow& l_wind); // Activates InputArea. (Call this when the area is clicked)
    void Stop(); // Deactivates InputArea. (Call this when something other than the area is clicked)
    void HandleTextEntered(sf::Uint32);

    bool IsActive();


    // Use this to determine if a mouse click is within the TextInputArea
    sf::FloatRect GetBounds();

private:
    Client* m_client;

    sf::RectangleShape m_backdrop;
    sf::Font m_font;
    sf::Text m_inputText;
    std::string m_inputString;
    int m_numVisible;
    bool m_active;

    void Send();
    void HandleEvent(sf::Event l_event);
};


#endif //CLIENT_WITHCHATBOX_TEXTINPUTAREA_H
