// Sort-Visualiser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>

#include <iostream>



int main()
{
    
    Window w(0.66);

    while (w.window.isOpen())
    {
        sf::Event event;
        while (w.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                w.window.close();
        }






        





    }

    return 0;
}


void draw(Window window1) {

    sf::Vector2f uiSize(window1.width, window1.height / 20);

    sf::RectangleShape uiBanner(uiSize);
    uiBanner.setPosition(0, 0);
    uiBanner.setFillColor(sf::Color::Green);

    window1.window.clear();
    window1.window.draw(uiBanner);
    window1.window.display();

}

class Window {

    public:

        Window(float scale) {
            width = sf::VideoMode::getDesktopMode().width * scale;
            height = sf::VideoMode::getDesktopMode().height * scale;
            sf::RenderWindow window(sf::VideoMode(width, height), "Sorting Algorithm Visualiser");
        }

        float scale;

        sf::RenderWindow window;
        int width = sf::VideoMode::getDesktopMode().width * scale;
        int height = sf::VideoMode::getDesktopMode().height * scale;
};








