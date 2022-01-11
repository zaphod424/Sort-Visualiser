// Sort-Visualiser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>

#include <iostream>

class Button {
public:
    int xpos;
    int width;
    std::string name;
    sf::RectangleShape icon;
    sf::Color colour;
    sf::Color textColour;


    Button(std::string a, int b, sf::Vector2f buttonSize) {

        name = a;
        xpos = b;
        sf::RectangleShape icon1(buttonSize);
        sf::Color colour = sf::Color::White;
        sf::Color textColour = sf::Color::Black;

        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, 0);
        icon.setOutlineColor(sf::Color::Black);
        icon.setOutlineThickness(2);

    }
};

int main()
{


    float scale = 0.6;

    int windowWidth = sf::VideoMode::getDesktopMode().width * scale;
    int windowHeight = sf::VideoMode::getDesktopMode().height * scale;

    sf::Vector2f uiSize(windowWidth, windowHeight / 20);
    sf::Vector2f buttonSize(windowWidth/10, windowHeight / 20);
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sorting Algorithm Visualiser");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Error no font";
    }


    Button rand("Randomise", 0, buttonSize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        sf::RectangleShape uiBanner(uiSize);
        uiBanner.setPosition(0, 0);
        uiBanner.setFillColor(sf::Color::Green);

        window.clear();
        window.draw(uiBanner);


        window.draw(rand.icon);
        sf::Text text(rand.name, font);
        text.setPosition(rand.xpos, 0);
        text.setFillColor(rand.textColour);
        window.draw(text);





        window.display();


        





    }

    return 0;
}












