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
        icon.setPosition(xpos, 2);
        icon.setOutlineColor(sf::Color::Black);
        icon.setOutlineThickness(2);

    }
};

int main()
{

    const int n_buttons = 7;
    float scale = 0.6;

    //setting window parameters
    int windowWidth = sf::VideoMode::getDesktopMode().width * scale;
    int windowHeight = sf::VideoMode::getDesktopMode().height * scale;


    
    // loading font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        std::cout << "Error no font";
    }

    //setting banner parameters
    sf::Vector2f uiSize(windowWidth, windowHeight / 20);
    sf::RectangleShape uiBanner(uiSize);
    uiBanner.setPosition(0, 0);
    uiBanner.setFillColor(sf::Color::Green);

    //setting button parameters
    sf::Vector2f buttonSize(windowWidth / n_buttons, windowHeight / 20);
    std::string buttonNameList[n_buttons] = { "Randomise", "Inc num bars", "Dec num bars", "Run Bogosort", "Run Bubblesort", "Run Quicksort", "Run Merge sort"};
    std::vector<Button> buttonList;

    //creating button objects
    for (int i = 0; i < n_buttons; i++) { 
        Button button(buttonNameList[i], buttonSize.x * i , buttonSize);
        buttonList.push_back(button);
    }

    //window loop
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sorting Algorithm Visualiser");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }



        //start clear_draw_display 
        window.clear();

        //drawing banner
        window.draw(uiBanner);

        //drawing buttons
        for (int i = 0; i < n_buttons; i++) {

            window.draw(buttonList[i].icon);
            sf::Text text(buttonList[i].name, font);
            text.setPosition(buttonList[i].xpos, 0);
            text.setFillColor(buttonList[i].textColour);
            window.draw(text);

        }


        window.display();       //end of clear_draw_display
        





    }

    return 0;
}












