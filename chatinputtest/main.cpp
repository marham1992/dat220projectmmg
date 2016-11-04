#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "chatbox.h"
#include "textinputarea.h"
#include "textbox.h"

// Simple structure (a class where everything is public) for our square
struct Square
{
    Square(float x, float y) : x(x), y(y) {}

    void move(int screenWidth, int screenHeight)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            y += speed;

        // Keep the box within screen borders
        x = std::max(x, 0.f);
        x = std::min(x, (float)(screenWidth - size));
        y = std::max(y, 0.f);
        y = std::min(y, (float)(screenHeight - size));
    }

    void draw(sf::RenderWindow& window)
    {
        // Create and position the rectangle used to draw the square
        sf::RectangleShape rectangle(sf::Vector2f(size, size));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::Red);

        // Draw the square
        window.draw(rectangle);
    }
    const int size = 100;
    const float speed = 5;
    float x, y;
};

int main()
{
    // Screen dimensions
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Create our window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Chat");

    // Locks the screen refresh to the monitor refresh (usually 60 fps)
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("LiberationSerif-Bold.ttf"))
    {
        std::cout << "error loading font" << std::endl;
    }
    /*sf::Text tekst1;
    tekst1.setFont(font);
    tekst1.setPosition(0,0);
    tekst1.setColor(sf::Color::Yellow);
    tekst1.setCharacterSize(24);
    tekst1.setString("TESTSTRING teststring");*/

    TextBox textbox12(sf::Vector2f(100,100),500,5,20,10,font);
    textbox12.setFillColor(sf::Color::White);
    textbox12.setOutlineColor(sf::Color::Black);
    textbox12.setCharColor(sf::Color::Black);

    //TESTDATA - DELETE
    textbox12.push("test1");
    textbox12.push("test2");
    textbox12.push("test3");
    textbox12.push("test4");
    textbox12.push("test5");
    textbox12.push("test6");
    textbox12.push("test7");
    textbox12.push("test8");
    textbox12.push("test9");
    textbox12.push("test10");
    textbox12.push("test11");



    TextInputArea chatbox12(sf::Vector2f(100, 350), 500, 5, 20, font);
    chatbox12.setFillColor(sf::Color::White);
    chatbox12.setOutlineColor(sf::Color::Black);
    chatbox12.setCharColor(sf::Color::Black);

    // Game loop, run while the window is open


    while (window.isOpen())
    {
        sf::Event event;

        // Get events from OS
        while (window.pollEvent(event))
        {
            // Close the window if the user clicks the x button
            if (event.type == sf::Event::Closed)
                window.close();

            chatbox12.handleEvent(event,window);
            textbox12.handleEvent(event,window);
        }


        //clear window
        window.clear(sf::Color::White);

        //Draw all
        textbox12.draw(window);

        chatbox12.update();
        chatbox12.draw(window);



        // Flip buffers to show what we've drawn
        window.display();
    }

    return 0;
}
