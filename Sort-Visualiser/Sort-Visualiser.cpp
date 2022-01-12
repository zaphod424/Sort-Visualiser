// Sort-Visualiser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>

#include <iostream>
#include <random>
#include <ctime>

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
        colour = sf::Color::White;
        textColour = sf::Color::Black;

        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, 2);
        icon.setOutlineColor(sf::Color::Black);
        icon.setOutlineThickness(2);

    }
};

class Bar {
public:
    int value, xpos, ypos;
    sf::Color colour;
    sf::RectangleShape icon;

    Bar(int v, sf::Vector2f barSize, int x, int y) {


        
        value = v;
        xpos = x;
        ypos = y;
        barSize.y = barSize.y * value;
        sf::RectangleShape icon1(barSize);
        colour = sf::Color::White;
        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, ypos-barSize.y);
    };

    void reposition(int x) {

        xpos = x;
        icon.setPosition(xpos, ypos);
    };

};

int main()
{

    const int n_buttons = 7;
    float scale = 0.6;
    int max_val = 10;

    //setting window parameters
    int windowWidth = sf::VideoMode::getDesktopMode().width * scale;
    int windowHeight = sf::VideoMode::getDesktopMode().height * scale;

    std::srand(std::time(nullptr));
    
    // loading font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        std::cout << "Error no font";
        return -1;
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


    //setting bar parameters

    int n_bars = 6;
    

    int ypos = windowHeight * 0.9;
    sf::Vector2f barSize(((windowWidth * 0.9) /n_bars) - 4 , (ypos - (uiSize.y * 2)) /max_val );

    std::vector<Bar> barList;
    

    //creating bar objects
    for (int i = 0; i < n_bars; i++) {
            
        int value = rand() % max_val;
        Bar newBar(value, barSize, ((windowWidth * 0.05)) + (barSize.x + 4)*i, ypos);
        barList.push_back(newBar);
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

        //drawing bars

        for (int i = 0; i < n_bars; i++) {

            window.draw(barList[i].icon);
        }

 



        window.display();       //end of clear_draw_display
        





    }

    return 0;
}












