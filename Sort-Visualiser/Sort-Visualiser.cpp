// Sort-Visualiser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>

#include <iostream>



int main()
{
    float scale = 0.666;

    int windowWidth = sf::VideoMode::getDesktopMode().width * scale;
    int windowHeight = sf::VideoMode::getDesktopMode().height *scale;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sorting Algorithm Visualiser");


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::vector2f uiSize(windowWidth, WindowHeight / 20);

        sf::RectangleShape uiBanner(uiSize);
        uiBanner.setPosition(0, 0);
        uiBanner.setFillColor(sf::Color::Green);


        window.clear();
        window.draw(uiBanner);
        window.display();

        





    }

    return 0;
}










